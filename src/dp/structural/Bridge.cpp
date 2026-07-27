#include <memory>
#include <utility>
#include "ExampleRegistry.h"
#include "Logger.h"

namespace {
namespace problem {
class Widget {
 public:
  virtual ~Widget() = default;
  virtual void click_on() const = 0;
};

class Button : public Widget {
 public:
  void click_on() const override { LOG("executed"); }
};

class ButtonWindows : public Button {
 public:
  void click_on() const override {
    LOG("executed");
    Button::click_on();
  }
};

class ButtonLinux : public Button {
 public:
  void click_on() const override {
    LOG("executed");
    Button::click_on();
  }
};

class Label : public Widget {
 public:
  void click_on() const override { LOG("executed"); }
};

class LabelWindows : public Label {
 public:
  void click_on() const override {
    LOG("executed");
    Label::click_on();
  }
};

class LabelLinux : public Label {
 public:
  void click_on() const override {
    LOG("executed");
    Label::click_on();
  }
};

void run() {
  LOG("Problem");
  // [Problem 1] We have to write the Text/TextLinux ...
  auto client_code = [](const Widget* widget) {
    if (widget != nullptr)
      widget->click_on();
  };

  // [Problem 2] : Use the Bridge if you need to be able to switch
  // implementations at runtime. how to exmaple for this still don't know
  Widget* button = new ButtonWindows();
  client_code(button);
  delete button;
}
}  // namespace problem

namespace bridge_pattern {
/// @class Implemetation Interface
/// @brief Define the interface for all implementation classes
class OsImplemetation {
 public:
  virtual void click_on_ipl() const = 0;
  virtual ~OsImplemetation() = default;
};

class WindowsImplemetation : public OsImplemetation {
 public:
  void click_on_ipl() const override { LOG("[Windows]"); }
};

class LinuxImplemetation : public OsImplemetation {
 public:
  void click_on_ipl() const override { LOG("[Linux]"); }
};

/// @class Abstractio Class
/// @brief Define the interface for the control part
class WidgetAbstraction {
 protected:
  std::shared_ptr<OsImplemetation> implementation_;

 public:
  explicit WidgetAbstraction(std::shared_ptr<OsImplemetation> implemetation)
      : implementation_{std::move(implemetation)} {}
  virtual ~WidgetAbstraction() = default;

  virtual void click_on() const = 0;
};

class ButtonAbstraction : public WidgetAbstraction {
 public:
  explicit ButtonAbstraction(std::shared_ptr<OsImplemetation> implemetation)
      : WidgetAbstraction{std::move(implemetation)} {}
  void click_on() const override {
    LOG("executed");
    this->implementation_->click_on_ipl();
  }
};

class LabelAbstraction : public WidgetAbstraction {
 public:
  explicit LabelAbstraction(std::shared_ptr<OsImplemetation> implemetation)
      : WidgetAbstraction{std::move(implemetation)} {}
  void click_on() const override {
    LOG("executed");
    this->implementation_->click_on_ipl();
  }
};

void run() {
  LOG("Bridge Example");
  auto client_code = [](const WidgetAbstraction* widget) {
    if (widget != nullptr) {
      LOG("");
      widget->click_on();
    }
  };

  {
    auto os = std::make_shared<WindowsImplemetation>();
    auto widget = std::make_unique<ButtonAbstraction>(os);
    client_code(widget.get());
  }

  {
    auto os = std::make_shared<LinuxImplemetation>();
    auto widget = std::make_unique<LabelAbstraction>(os);
    client_code(widget.get());
  }
}
}  // namespace bridge_pattern

class BridgeExample : public IExample {
 public:
  std::string group() const override { return "dp/structural"; }
  std::string name() const override { return "Bridge"; }
  std::string description() const override { return "Bridge Pattern Example"; }
  void execute() override {
    problem::run();
    bridge_pattern::run();
  }
};

REGISTER_EXAMPLE(BridgeExample);
}  // namespace