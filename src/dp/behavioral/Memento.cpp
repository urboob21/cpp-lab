// -----------------------------------------------------------------------------
// Memento (behavioral pattern)
//
// Captures an object's internal state so it can be restored later, WITHOUT
// exposing that state to the rest of the program.
//
// Problem: to implement undo, some other object (the history) would need to
// read and write the editor's private fields - breaking encapsulation.
// Solution: the editor itself creates an opaque snapshot (memento). The history
// only stores snapshots; only the editor can look inside them.
//
// Use it when:
//   - you need snapshots to undo or roll back
//   - direct access to the fields would break encapsulation
//
// Participants:
//   Originator  Editor - creates and restores mementos
//   Memento     Editor::Snapshot - state is private, readable only by Editor
//   Caretaker   History - keeps mementos, never inspects them
//
// UML: docs/uml/dp/behavioral_memento.drawio.svg
// -----------------------------------------------------------------------------

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Originator.
class Editor {
 public:
  /// Memento: public metadata, private state.
  class Snapshot {
   public:
    const std::string& label() const { return label_; }  // safe to show

   private:
    friend class Editor;  // only the originator can read the state
    Snapshot(std::string text, std::size_t cursor, std::string label)
        : text_{std::move(text)}, cursor_{cursor}, label_{std::move(label)} {}

    std::string text_;
    std::size_t cursor_;
    std::string label_;
  };

  void type(const std::string& text) {
    text_.insert(cursor_, text);
    cursor_ += text.size();
  }

  void moveCursor(std::size_t position) {
    cursor_ = std::min(position, text_.size());
  }

  std::unique_ptr<Snapshot> save(const std::string& label) const {
    // std::make_unique cannot reach the private constructor, so `new` is used.
    return std::unique_ptr<Snapshot>(new Snapshot(text_, cursor_, label));
  }

  void restore(const Snapshot& snapshot) {
    text_ = snapshot.text_;
    cursor_ = snapshot.cursor_;
  }

  std::string show() const {
    std::string view = text_;
    view.insert(cursor_, "|");  // visualize the cursor
    return "\"" + view + "\"";
  }

 private:
  std::string text_;
  std::size_t cursor_{0};
};

/// Caretaker: stores snapshots, has no idea what they contain.
class History {
 public:
  explicit History(Editor& editor) : editor_{editor} {}

  void backup(const std::string& label) {
    snapshots_.push_back(editor_.save(label));
  }

  void undo() {
    if (snapshots_.empty()) {
      LOG("  nothing to undo");
      return;
    }
    const std::unique_ptr<Editor::Snapshot> snapshot =
        std::move(snapshots_.back());
    snapshots_.pop_back();
    editor_.restore(*snapshot);
    LOG_S("  undo to '" << snapshot->label() << "' -> " << editor_.show());
  }

  void list() const {
    for (const auto& snapshot : snapshots_) {
      LOG_S("    saved: " << snapshot->label());
    }
  }

 private:
  Editor& editor_;
  std::vector<std::unique_ptr<Editor::Snapshot>> snapshots_;
};

void run() {
  Editor editor;
  History history{editor};

  LOG_SECTION("Editing and taking snapshots");
  history.backup("empty");
  editor.type("Hello");
  LOG_S("  typed           -> " << editor.show());

  history.backup("after Hello");
  editor.type(" World");
  LOG_S("  typed           -> " << editor.show());

  history.backup("after World");
  editor.moveCursor(5);
  editor.type(",");
  LOG_S("  inserted comma  -> " << editor.show());
  history.list();

  LOG_SECTION("Undo");
  history.undo();
  history.undo();
  history.undo();
  history.undo();
  // history.snapshots_[0]->text_;  // error: private - encapsulation is preserved
}

}  // namespace

LAB_EXAMPLE("Memento",
            "save and restore state (undo) without breaking encapsulation") {
  run();
}
