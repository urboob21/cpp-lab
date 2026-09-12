#pragma once

#include <gtkmm.h>
#include <memory>
#include "../IObserver.h"
#include "../viewmodel/SharedDataVM.h"

namespace mvvm {
class DisplayWidget : public Gtk::Box, public IObserver {
 public:
  DisplayWidget(const std::string& title, std::string color,
                std::shared_ptr<SharedDataVM> vm);

  void onDataChanged(const std::string& newData) override;

 private:
  void updateLabel(const std::string& text);

  std::string color_;
  Gtk::Frame frame_;
  Gtk::Box innerBox_;
  Gtk::Label labelData_;

  std::shared_ptr<SharedDataVM> view_model_;
};
}  // namespace mvvm