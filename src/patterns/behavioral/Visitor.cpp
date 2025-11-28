// Visitor is a behavioral design pattern that lets you separate algorithms from
// the objects on which they operate. Appicability:
// (*)   when you need to perform an operation on all elements of a complex
// object structure (for example, an object tree, document).
// (**)  to clean up the business logic of auxiliary behaviors.
// (**)  when a behavior makes sense only in some classes of a class hierarchy,
// but not in others. UML: docs/uml/patterns_behavioral_visitor.drawio.svg

#include <iostream>
#include <string>
#include <vector>

namespace {
namespace Visitor {
class TextConcreteElement;
class ImageConcreteElement;
class TableConcreteElement;

/**
 * The Visitor interface declares a set of visiting methods that can take
 * concrete elements of an object structure as arguments. These methods may have
 * the same names if the program is written in a language that supports
 * overloading, but the type of their parameters must be different.
 */
class IVisitor {
 public:
  virtual ~IVisitor() = default;

  virtual void visitText(const TextConcreteElement* t) = 0;
  virtual void visitImage(const ImageConcreteElement* i) = 0;
  virtual void visitTable(const TableConcreteElement* t) = 0;
};

/**
 * The Element interface declares a method for “accepting” visitors.
 * This method should have one parameter declared with the type of the visitor
 * interface.
 */
class IElement {
 public:
  virtual ~IElement() = default;
  // [P1]
  // if we implement exportHtml(), exportMarkdown(), exportJson(), etc.. here,
  // every time we add a new export format we must modify ALL element classes.
  // This violates the Open/Closed Principle
  // and makes each element hold multiple unrelated behaviors.
  // => Use `Visitor Pattern` to separate export logic from the element classes.

  virtual void accept(IVisitor* visitor) = 0;
};

/**
 * The complex object structure
 */
class DocumentConcreteStructure {
 private:
  std::vector<IElement*> m_elements;

 public:
  ~DocumentConcreteStructure() {
    for (IElement* e : this->m_elements) {
      delete e;
    }
    m_elements.clear();
  }

  void add(IElement* e) { m_elements.push_back(e); }

  std::vector<IElement*>& get() { return this->m_elements; };
};

/**
 * Each Concrete Element must implement the acceptance method.
 * The purpose of this method is to redirect the call to the proper visitor’s
 * method corresponding to the current element class. Be aware that even if a
 * base element class implements this method, all subclasses must still override
 * this method in their own classes and call the appropriate method on the
 * visitor object.
 */
class TextConcreteElement : public IElement {
 private:
  std::string m_content;

 public:
  explicit TextConcreteElement(const std::string& c) : m_content{c} {};
  std::string getContent() const { return this->m_content; }

  void accept(IVisitor* visitor) override { visitor->visitText(this); }
};

class ImageConcreteElement : public IElement {
 private:
  std::string m_path;

 public:
  explicit ImageConcreteElement(const std::string& p) : m_path{p} {};

  std::string getPath() const { return this->m_path; }

  void accept(IVisitor* visitor) override { visitor->visitImage(this); }
};

class TableConcreteElement : public IElement {
 private:
  int m_rows, m_cols;

 public:
  TableConcreteElement(int r, int c) : m_rows{r}, m_cols{c} {};
  int getRows() const { return this->m_rows; }
  int getCols() const { return this->m_cols; }

  void accept(IVisitor* visitor) override { visitor->visitTable(this); }
};

/**
 * Each Concrete Visitor implements several versions of the same behaviors,
 * tailored for different concrete element classes.
 */
class HtmlExportConcreteVisitor : public IVisitor {
  void visitText(const TextConcreteElement* t) override {
    std::cout << "<p>" + t->getContent() + "</p>\n";
  }

  void visitImage(const ImageConcreteElement* i) override {
    std::cout << "<img src=\"" + i->getPath() + "\" />\n";
  }

  void visitTable(const TableConcreteElement* t) override {
    std::cout << "<table>\n";
    for (int r = 0; r < t->getRows(); ++r) {
      std::cout << "  <tr>";
      for (int c = 0; c < t->getCols(); ++c) {
        std::cout << "<td>cell</td>";
      }
      std::cout << "</tr>\n";
    }
    std::cout << "</table>\n";
  }
};

class JsonExportConcreteVisitor : public IVisitor {
  void visitText(const TextConcreteElement* t) override {
    std::cout << "{ \"type\": \"text\", \"content\": \"" << t->getContent()
              << "\" }\n";
  }
  void visitImage(const ImageConcreteElement* i) override {
    std::cout << "{ \"type\": \"image\", \"path\": \"" << i->getPath()
              << "\" }\n";
  }

  void visitTable(const TableConcreteElement* t) override {
    std::cout << "{ \"type\": \"table\", \"rows\": " << t->getRows()
              << ", \"cols\": " << t->getCols() << " }\n";
  }
};

class MarkdownExportConcreteVisitor : public IVisitor {
  void visitText(const TextConcreteElement* t) override {
    std::cout << t->getContent() << "\n\n";
  }

  void visitImage(const ImageConcreteElement* i) override {
    std::cout << "![](" << i->getPath() << ")\n\n";
  }

  void visitTable(const TableConcreteElement* t) override {
    for (int r = 0; r < t->getRows(); ++r) {
      for (int c = 0; c < t->getCols(); ++c) {
        std::cout << "| cell ";
      }
      std::cout << "|\n";
    }
    std::cout << "\n";
  }
};

/**
 * clients aren’t aware of all the concrete element classes because they work
 * with objects from that collection via some abstract interface.
 */
namespace Client {
void clientCode(DocumentConcreteStructure* const doc, IVisitor* const visitor) {
  // [P2] Instead of adding exportHtml(), exportMarkdown(), exportJson() in each
  // element, we use the Visitor pattern to separate data from behavior. This
  // avoids bloating element classes with multiple unrelated functions and makes
  // it easier to add new export formats without modifying the elements.
  for (IElement* ele : doc->get()) {
    ele->accept(visitor);
  }
}
}  // namespace Client
void run() {
  DocumentConcreteStructure* document = new DocumentConcreteStructure();
  // Add images
  document->add(new ImageConcreteElement("header.png"));
  document->add(new ImageConcreteElement("diagram1.png"));
  document->add(new ImageConcreteElement("chart.png"));

  // Add text paragraphs
  document->add(new TextConcreteElement(
      "Introduction: This document demonstrates the Visitor pattern."));
  document->add(
      new TextConcreteElement("Section 1: Visitor allows adding new operations "
                              "without modifying elements."));
  document->add(new TextConcreteElement(
      "Section 2: Elements are data holders, visitors implement behaviors."));

  // Add tables
  document->add(
      new TableConcreteElement(3, 4));  // e.g., 3 rows x 4 columns table
  document->add(new TableConcreteElement(2, 2));  // smaller table
  document->add(new TableConcreteElement(4, 3));  // another table

  // Add more text
  document->add(new TextConcreteElement(
      "Conclusion: Using Visitor pattern makes code open for extension but "
      "closed for modification."));

  IVisitor* visitor = new HtmlExportConcreteVisitor();
  std::cout << " ===HTML Export ===\n";
  Client::clientCode(document, visitor);
  std::cout << " ==================\n";

  visitor = new JsonExportConcreteVisitor();
  std::cout << " ===JSON Export ===\n";
  Client::clientCode(document, visitor);
  std::cout << " ==================\n";

  visitor = new MarkdownExportConcreteVisitor();
  std::cout << " ===MD Export ===\n";
  Client::clientCode(document, visitor);
  std::cout << " ==================\n";
  delete visitor;
  delete document;
}
}  // namespace Visitor
}  // namespace

struct VisitorAutoRunner {
  VisitorAutoRunner() {
    std::cout << "\n--- Visitor Pattern Example ---\n";
    Visitor::run();
  }
};

static VisitorAutoRunner instance;