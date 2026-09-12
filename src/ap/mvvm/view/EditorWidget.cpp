#include "EditorWidget.h"

#include <utility>

mvvm::EditorWidget::EditorWidget(std::shared_ptr<SharedDataVM> vm)
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      innerBox_(Gtk::Orientation::VERTICAL),
      view_model_(std::move(vm)) {
  frame_.set_label("ZONE 1: EDITOR (Input View)");
  frame_.set_margin(10);

  labelTitle_.set_text("Enter new data:");
  entry_.set_text(view_model_->getCurrentText());
  button_.set_label("Broadcast Update");

  innerBox_.append(labelTitle_);
  innerBox_.append(entry_);
  innerBox_.append(button_);
  innerBox_.set_margin(15);
  innerBox_.set_spacing(10);

  frame_.set_child(innerBox_);
  this->append(frame_);

  // MVVM binding: user action -> ViewModel command
  button_.signal_clicked().connect(
      [this]() { view_model_->submitText(entry_.get_text()); });

  // Self-register
  view_model_->addObserver(this);
}

void mvvm::EditorWidget::onDataChanged(const std::string& newData) {
  // Only touch the entry when the text really changed: set_text() would move
  // the cursor and could start another round of notifications.
  if (entry_.get_text() != Glib::ustring(newData)) {
    entry_.set_text(newData);
  }
}