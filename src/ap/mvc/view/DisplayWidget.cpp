#include "DisplayWidget.h"

#include <utility>

DisplayWidget::DisplayWidget(const std::string& title, std::string color,
                             const std::string& startData)
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      color_(std::move(color)),
      innerBox_(Gtk::Orientation::VERTICAL) {
  frame_.set_label(title);
  frame_.set_margin(10);

  updateLabel(startData);

  innerBox_.append(labelData_);
  innerBox_.set_margin(20);

  frame_.set_child(innerBox_);
  this->append(frame_);
}

void DisplayWidget::updateLabel(const std::string& text) {
  // Pango markup changes color and size. The text comes from the user, so it
  // must be escaped: a '<' or '&' would otherwise break the markup.
  const std::string markup = "<span foreground='" + color_ +
                             "' size='x-large' weight='bold'>" +
                             Glib::Markup::escape_text(text).raw() + "</span>";
  labelData_.set_markup(markup);
}

void DisplayWidget::onDataChanged(const std::string& newData) {
  updateLabel(newData);
}