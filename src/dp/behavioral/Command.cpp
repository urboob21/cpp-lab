// -----------------------------------------------------------------------------
// Command (behavioral pattern)
//
// Turns a request into a stand-alone object that contains everything needed to
// perform it (receiver + arguments). Requests can then be passed around, queued,
// logged, combined into macros and UNDONE.
//
// Use it when:
//   - UI elements (buttons, shortcuts, menus) should trigger operations without
//     knowing how they are implemented
//   - operations must be queued, scheduled or executed remotely
//   - operations must be reversible (undo/redo)
//
// Participants:
//   Command          interface with execute() / undo()
//   ConcreteCommand  InsertTextCommand, ClearCommand, MacroCommand
//   Receiver         TextDocument - does the real work
//   Invoker          Editor - runs commands and keeps the history
//   Client           creates commands and hands them to the invoker
//
// UML: docs/uml/dp/behavioral_command.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Receiver: contains the business logic.
class TextDocument {
 public:
  void insert(const std::string& text) { content_ += text; }
  void eraseLast(std::size_t count) { content_.erase(content_.size() - count); }
  const std::string& content() const { return content_; }
  void setContent(std::string content) { content_ = std::move(content); }

 private:
  std::string content_;
};

/// Command interface.
class Command {
 public:
  virtual ~Command() = default;
  virtual void execute() = 0;
  virtual void undo() = 0;
  virtual std::string name() const = 0;
};

class InsertTextCommand : public Command {
 public:
  InsertTextCommand(TextDocument& document, std::string text)
      : document_{document}, text_{std::move(text)} {}

  void execute() override { document_.insert(text_); }
  void undo() override { document_.eraseLast(text_.size()); }
  std::string name() const override { return "insert \"" + text_ + "\""; }

 private:
  TextDocument& document_;
  std::string text_;
};

class ClearCommand : public Command {
 public:
  explicit ClearCommand(TextDocument& document) : document_{document} {}

  void execute() override {
    backup_ = document_.content();  // remember what is needed to undo
    document_.setContent("");
  }
  void undo() override { document_.setContent(backup_); }
  std::string name() const override { return "clear"; }

 private:
  TextDocument& document_;
  std::string backup_;
};

/// A command made of other commands.
class MacroCommand : public Command {
 public:
  void add(std::unique_ptr<Command> command) {
    commands_.push_back(std::move(command));
  }

  void execute() override {
    for (const auto& command : commands_) {
      command->execute();
    }
  }
  void undo() override {  // undo in reverse order
    for (auto it = commands_.rbegin(); it != commands_.rend(); ++it) {
      (*it)->undo();
    }
  }
  std::string name() const override {
    return "macro of " + std::to_string(commands_.size());
  }

 private:
  std::vector<std::unique_ptr<Command>> commands_;
};

/// Invoker: executes commands without knowing what they do, and keeps a history.
class Editor {
 public:
  explicit Editor(const TextDocument& document) : document_{document} {}

  void run(std::unique_ptr<Command> command) {
    command->execute();
    LOG_S("  run  " << command->name() << " -> \"" << document_.content()
                    << "\"");
    history_.push_back(std::move(command));
  }

  void undo() {
    if (history_.empty()) {
      LOG("  undo: nothing to undo");
      return;
    }
    history_.back()->undo();
    LOG_S("  undo " << history_.back()->name() << " -> \""
                    << document_.content() << "\"");
    history_.pop_back();
  }

 private:
  const TextDocument& document_;
  std::vector<std::unique_ptr<Command>> history_;
};

void run() {
  TextDocument document;
  Editor editor{document};

  LOG_SECTION("Executing commands");
  editor.run(std::make_unique<InsertTextCommand>(document, "Hello"));
  editor.run(std::make_unique<InsertTextCommand>(document, " World"));
  editor.run(std::make_unique<ClearCommand>(document));

  LOG_SECTION("Undo");
  editor.undo();  // restores "Hello World"
  editor.undo();  // removes " World"

  LOG_SECTION("Macro command: one undo step for several commands");
  auto signature = std::make_unique<MacroCommand>();
  signature->add(std::make_unique<InsertTextCommand>(document, "\n--\n"));
  signature->add(std::make_unique<InsertTextCommand>(document, "Best regards"));
  editor.run(std::move(signature));
  editor.undo();
  editor.undo();
  editor.undo();
}

}  // namespace

LAB_EXAMPLE("Command",
            "requests as objects: execute, undo history and macro commands") {
  run();
}
