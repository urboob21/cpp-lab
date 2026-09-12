#pragma once
#include <gtkmm.h>
#include "../IObserver.h"
#include "../controller/Controller.h"

class EditorWidget : public Gtk::Box, public IObserver {
 public:
  EditorWidget(std::shared_ptr<Controller> c, const std::string& initData);

  void onDataChanged(const std::string& newData) override;

 private:
  Gtk::Frame frame_;
  Gtk::Box innerBox_;
  Gtk::Label labelTitle_;
  Gtk::Entry entry_;
  Gtk::Button button_;

  std::shared_ptr<Controller> controller_;
};