#include "Controller.h"

#include <utility>

Controller::Controller(std::shared_ptr<SharedData> model)
    : model_(std::move(model)) {}

void Controller::updateRequest(const std::string& text) {
  if (text.empty())
    return;

  // the Controller updates the Model when receiving new data from the View,
  // which then notifies all Views of the change
  model_->setData(text);
}