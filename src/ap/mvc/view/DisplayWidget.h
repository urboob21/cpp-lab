#pragma once

#include <gtkmm.h>
#include "../IObserver.h"

class DisplayWidget : public Gtk::Box, public IObserver {
 public:
  DisplayWidget(const std::string& title, const std::string& color,
                const std::string& startData);

  void onDataChanged(const std::string& newData) override;

 private:
  void updateLabel(const std::string& text);

  std::string color_;
  Gtk::Frame frame_;
  Gtk::Box innerBox_;
  Gtk::Label labelData_;
};