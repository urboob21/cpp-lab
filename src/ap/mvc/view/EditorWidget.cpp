#include "EditorWidget.h"

#include <utility>

EditorWidget::EditorWidget(std::shared_ptr<Controller> c,
                           const std::string& initData)
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      innerBox_(Gtk::Orientation::VERTICAL),
      controller_(std::move(c)) {
  // Create a beautiful border
  frame_.set_label("ZONE 1: EDITOR (Input View)");
  frame_.set_margin(10);

  labelTitle_.set_text("Enter new data:");
  entry_.set_text(initData);
  button_.set_label("Broadcast Update");

  // Layout inside the frame
  innerBox_.append(labelTitle_);
  innerBox_.append(entry_);
  innerBox_.append(button_);
  innerBox_.set_margin(15);
  innerBox_.set_spacing(10);

  frame_.set_child(innerBox_);
  this->append(frame_);  // Add a frame to the main Box of this class

  // MVC logic to help the Controller receive update input from the View
  button_.signal_clicked().connect(
      [this]() { controller_->updateRequest(entry_.get_text()); });
}

void EditorWidget::onDataChanged(const std::string& newData) {
  // Only touch the entry when the text really changed: set_text() would move
  // the cursor and could start another round of notifications.
  if (entry_.get_text() != Glib::ustring(newData)) {
    entry_.set_text(newData);
  }
}