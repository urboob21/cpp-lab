// -----------------------------------------------------------------------------
// Visitor (behavioral pattern)
//
// Separates operations from the objects they work on. New operations become
// new visitor classes; the element classes stay untouched.
//
// Problem: adding exportHtml(), exportJson(), exportMarkdown()... to every
// element class means modifying ALL of them for each new format, and each
// element collects unrelated responsibilities.
// Solution: each element only implements accept(visitor), which calls the
// visitor method for its concrete type ("double dispatch").
//
// Use it when:
//   - you need many unrelated operations on a stable set of element types
//   - an operation only makes sense for some classes of a hierarchy
// Trade-off: adding a new ELEMENT type requires changing every visitor.
//
// Participants:
//   Visitor          interface with one visit method per element type
//   ConcreteVisitor  HtmlExporter, JsonExporter, WordCounter
//   Element          accept(Visitor&)
//   ConcreteElement  Paragraph, Image, Table
//
// Modern C++ alternative for a closed set of types: std::variant + std::visit.
//
// UML: docs/uml/dp/behavioral_visitor.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace classic {

class Paragraph;
class Image;
class Table;

/// Visitor interface.
class Visitor {
 public:
  virtual ~Visitor() = default;
  virtual void visit(const Paragraph& paragraph) = 0;
  virtual void visit(const Image& image) = 0;
  virtual void visit(const Table& table) = 0;
};

/// Element interface.
class Element {
 public:
  virtual ~Element() = default;
  virtual void accept(Visitor& visitor) const = 0;
};

class Paragraph : public Element {
 public:
  explicit Paragraph(std::string text) : text_{std::move(text)} {}
  const std::string& text() const { return text_; }
  // Second dispatch: `*this` has the static type Paragraph here.
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

 private:
  std::string text_;
};

class Image : public Element {
 public:
  explicit Image(std::string path) : path_{std::move(path)} {}
  const std::string& path() const { return path_; }
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

 private:
  std::string path_;
};

class Table : public Element {
 public:
  Table(int rows, int columns) : rows_{rows}, columns_{columns} {}
  int rows() const { return rows_; }
  int columns() const { return columns_; }
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

 private:
  int rows_;
  int columns_;
};

class HtmlExporter : public Visitor {
 public:
  void visit(const Paragraph& paragraph) override {
    LOG_S("    <p>" << paragraph.text() << "</p>");
  }
  void visit(const Image& image) override {
    LOG_S("    <img src=\"" << image.path() << "\"/>");
  }
  void visit(const Table& table) override {
    LOG_S("    <table rows=\"" << table.rows() << "\" cols=\""
                               << table.columns() << "\"/>");
  }
};

class JsonExporter : public Visitor {
 public:
  void visit(const Paragraph& paragraph) override {
    LOG_S(R"(    {"type": "text", "content": ")" << paragraph.text() << "\"}");
  }
  void visit(const Image& image) override {
    LOG_S(R"(    {"type": "image", "path": ")" << image.path() << "\"}");
  }
  void visit(const Table& table) override {
    LOG_S(R"(    {"type": "table", "rows": )"
          << table.rows() << ", \"cols\": " << table.columns() << '}');
  }
};

/// A visitor that computes something instead of printing.
class WordCounter : public Visitor {
 public:
  void visit(const Paragraph& paragraph) override {
    std::istringstream words(paragraph.text());
    for (std::string word; words >> word;) {
      ++count_;
    }
  }
  void visit(const Image& /*image*/) override {}
  void visit(const Table& /*table*/) override {}
  int count() const { return count_; }

 private:
  int count_{0};
};

void run() {
  std::vector<std::unique_ptr<Element>> document;
  document.push_back(
      std::make_unique<Paragraph>("Visitor separates data from behavior."));
  document.push_back(std::make_unique<Image>("diagram.png"));
  document.push_back(std::make_unique<Table>(3, 4));
  document.push_back(
      std::make_unique<Paragraph>("New exports need no element changes."));

  const auto apply = [&document](Visitor& visitor) {
    for (const auto& element : document) {
      element->accept(visitor);
    }
  };

  LOG_SECTION("HTML export");
  HtmlExporter html;
  apply(html);

  LOG_SECTION("JSON export");
  JsonExporter json;
  apply(json);

  LOG_SECTION("Word count (a visitor that collects data)");
  WordCounter counter;
  apply(counter);
  LOG_S("    words in paragraphs: " << counter.count());
}

}  // namespace classic

namespace with_variant {

struct Paragraph {
  std::string text;
};
struct Image {
  std::string path;
};

using Element = std::variant<Paragraph, Image>;

/// Helper that merges several lambdas into one overloaded function object.
template <typename... Lambdas>
struct Overloaded : Lambdas... {
  using Lambdas::operator()...;
};

void run() {
  LOG_SECTION("Modern alternative: std::variant + std::visit");
  const std::vector<Element> document{Paragraph{"no base classes needed"},
                                      Image{"photo.jpg"}};
  for (const Element& element : document) {
    std::visit(
        Overloaded{
            [](const Paragraph& p) { LOG_S("    paragraph: " << p.text); },
            [](const Image& i) { LOG_S("    image: " << i.path); },
        },
        element);
  }
}

}  // namespace with_variant

}  // namespace

LAB_EXAMPLE("Visitor",
            "add operations without changing element classes; std::variant "
            "alternative") {
  classic::run();
  with_variant::run();
}
