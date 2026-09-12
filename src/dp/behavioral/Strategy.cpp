// -----------------------------------------------------------------------------
// Strategy (behavioral pattern)
//
// Defines a family of algorithms, puts each into its own class and makes them
// interchangeable at run time.
//
// Problem:
//   std::string exportDocument(const std::string& format) {
//     if (format == "html") { ... } else if (format == "json") { ... } ...
//   }
//   - data (the document) and behavior (every export format) are mixed
//   - each new format modifies this function (violates Open/Closed)
// Solution: every format is a strategy; the context only holds the data and
// the currently selected strategy.
//
// Use it when:
//   - you need different variants of an algorithm and want to switch at run time
//   - many classes only differ in how they execute some behavior
//   - a class has a massive conditional selecting an algorithm
//
// Participants:
//   Strategy          ExportStrategy interface
//   ConcreteStrategy  HtmlExport, JsonExport, MarkdownExport
//   Context           Document - uses a strategy through the interface
//
// Modern C++ alternative: store a std::function when a strategy is just one
// function (shown at the end).
//
// UML: docs/uml/dp/behavioral_strategy.drawio.svg
// -----------------------------------------------------------------------------

#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Strategy interface.
class ExportStrategy {
 public:
  virtual ~ExportStrategy() = default;
  virtual std::string exportText(const std::string& content) const = 0;
};

class HtmlExport : public ExportStrategy {
 public:
  std::string exportText(const std::string& content) const override {
    return "<p>" + content + "</p>";
  }
};

class JsonExport : public ExportStrategy {
 public:
  std::string exportText(const std::string& content) const override {
    return R"({"content": ")" + content + "\"}";
  }
};

class MarkdownExport : public ExportStrategy {
 public:
  std::string exportText(const std::string& content) const override {
    return "**" + content + "**";
  }
};

/// Context.
class Document {
 public:
  explicit Document(std::string content) : content_{std::move(content)} {}

  void setExportStrategy(std::unique_ptr<ExportStrategy> strategy) {
    strategy_ = std::move(strategy);
  }

  std::string exportDocument() const {
    if (!strategy_) {
      return "(no export strategy selected)";
    }
    return strategy_->exportText(content_);
  }

 private:
  std::string content_;
  std::unique_ptr<ExportStrategy> strategy_;
};

void classicStrategy() {
  LOG_SECTION("Swapping strategy objects at run time");
  Document report{"quarterly report"};
  LOG_S("  none     : " << report.exportDocument());

  report.setExportStrategy(std::make_unique<HtmlExport>());
  LOG_S("  HTML     : " << report.exportDocument());

  report.setExportStrategy(std::make_unique<JsonExport>());
  LOG_S("  JSON     : " << report.exportDocument());

  report.setExportStrategy(std::make_unique<MarkdownExport>());
  LOG_S("  Markdown : " << report.exportDocument());
}

void functionStrategy() {
  LOG_SECTION("Lightweight strategies with std::function");
  using Formatter = std::function<std::string(const std::string&)>;
  const std::string content = "release notes";

  Formatter upper = [](const std::string& text) {
    std::string result;
    std::transform(
        text.begin(), text.end(), std::back_inserter(result),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return result;
  };
  Formatter quoted = [](const std::string& text) {
    return "\"" + text + "\"";
  };

  for (const auto& [name, format] :
       {std::pair{"upper ", upper}, std::pair{"quoted", quoted}}) {
    LOG_S("  " << name << " : " << format(content));
  }
}

}  // namespace

LAB_EXAMPLE(
    "Strategy",
    "interchangeable algorithms behind an interface or a std::function") {
  classicStrategy();
  functionStrategy();
}
