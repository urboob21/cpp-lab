#include "DisplayWidget.h"

mvvm::DisplayWidget::DisplayWidget(const std::string& title, std::string color,
                                   std::shared_ptr<SharedDataVM> vm)
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      color_(std::move(color)),
      innerBox_(Gtk::Orientation::VERTICAL),
      view_model_(std::move(vm)) {
  frame_.set_label(title);
  frame_.set_margin(10);

  // get from the ViewModel's current state
  updateLabel(view_model_->getCurrentText());

  innerBox_.append(labelData_);
  innerBox_.set_margin(20);

  frame_.set_child(innerBox_);
  this->append(frame_);

  // self-register: now the VM will push future update to this View
  view_model_->addObserver(this);
}

void mvvm::DisplayWidget::updateLabel(const std::string& text) {
  // The text comes from the user: escape it before building Pango markup.
  const std::string markup = "<span foreground='" + color_ +
                             "' size='x-large' weight='bold'>" +
                             Glib::Markup::escape_text(text).raw() + "</span>";
  labelData_.set_markup(markup);
}

void mvvm::DisplayWidget::onDataChanged(const std::string& newData) {
  updateLabel(newData);
}