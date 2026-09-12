// -----------------------------------------------------------------------------
// Bridge (structural pattern)
//
// Splits one large class hierarchy into two independent ones - the
// ABSTRACTION (what the client uses) and the IMPLEMENTATION (how it is done) -
// connected by composition.
//
// Problem: widgets x platforms as subclasses
//   Button, WindowsButton, LinuxButton, Label, WindowsLabel, LinuxLabel, ...
//   Every new widget needs one class per platform and vice versa: M x N classes.
// Solution: Widget HAS-A Renderer. Widgets and renderers vary independently:
//   M + N classes, and the renderer can even be switched at run time.
//
// Participants:
//   Abstraction         Widget (holds a Renderer)
//   RefinedAbstraction  Button, Label
//   Implementor         Renderer interface
//   ConcreteImplementor WindowsRenderer, LinuxRenderer
//
// UML: docs/uml/dp/structural_bridge.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace problem {

class Widget {
 public:
  virtual ~Widget() = default;
  virtual void draw() const = 0;
};

class Button : public Widget {};
class WindowsButton : public Button {
 public:
  void draw() const override { LOG("    WindowsButton::draw"); }
};
class LinuxButton : public Button {
 public:
  void draw() const override { LOG("    LinuxButton::draw"); }
};
class Label : public Widget {};
class WindowsLabel : public Label {
 public:
  void draw() const override { LOG("    WindowsLabel::draw"); }
};
class LinuxLabel : public Label {
 public:
  void draw() const override { LOG("    LinuxLabel::draw"); }
};
// Adding MacOs means MacOsButton, MacOsLabel, ...; adding a Slider means
// WindowsSlider, LinuxSlider, ... The hierarchy grows in two directions.

void run() {
  LOG_SECTION("Problem: one subclass per widget AND platform");
  const std::unique_ptr<Widget> widgets[] = {std::make_unique<WindowsButton>(),
                                             std::make_unique<LinuxLabel>()};
  for (const auto& widget : widgets) {
    widget->draw();
  }
}

}  // namespace problem

namespace bridge {

/// Implementor: platform-specific primitives.
class Renderer {
 public:
  virtual ~Renderer() = default;
  virtual void drawRectangle(const std::string& label) const = 0;
  virtual void drawText(const std::string& text) const = 0;
};

class WindowsRenderer : public Renderer {
 public:
  void drawRectangle(const std::string& label) const override {
    LOG_S("    [Win32] DrawRectangle for '" << label << "'");
  }
  void drawText(const std::string& text) const override {
    LOG_S("    [Win32] DrawText '" << text << "'");
  }
};

class LinuxRenderer : public Renderer {
 public:
  void drawRectangle(const std::string& label) const override {
    LOG_S("    [X11] XDrawRectangle for '" << label << "'");
  }
  void drawText(const std::string& text) const override {
    LOG_S("    [X11] XDrawString '" << text << "'");
  }
};

/// Abstraction: high-level widget logic, delegates primitives to the renderer.
class Widget {
 public:
  explicit Widget(std::shared_ptr<const Renderer> renderer)
      : renderer_{std::move(renderer)} {}
  virtual ~Widget() = default;

  virtual void draw() const = 0;
  void setRenderer(std::shared_ptr<const Renderer> renderer) {
    renderer_ = std::move(renderer);
  }

 protected:
  const Renderer& renderer() const { return *renderer_; }

 private:
  std::shared_ptr<const Renderer> renderer_;  // the "bridge"
};

/// Refined abstractions.
class Button : public Widget {
 public:
  Button(std::shared_ptr<const Renderer> renderer, std::string caption)
      : Widget{std::move(renderer)}, caption_{std::move(caption)} {}
  void draw() const override {
    renderer().drawRectangle(caption_);
    renderer().drawText(caption_);
  }

 private:
  std::string caption_;
};

class Label : public Widget {
 public:
  Label(std::shared_ptr<const Renderer> renderer, std::string text)
      : Widget{std::move(renderer)}, text_{std::move(text)} {}
  void draw() const override { renderer().drawText(text_); }

 private:
  std::string text_;
};

void run() {
  LOG_SECTION("Bridge: widgets and renderers vary independently");
  const auto windows = std::make_shared<WindowsRenderer>();
  const auto linux_renderer = std::make_shared<LinuxRenderer>();

  Button ok{windows, "OK"};
  const Label title{linux_renderer, "Settings"};
  ok.draw();
  title.draw();

  LOG("  switching the button's renderer at run time:");
  ok.setRenderer(linux_renderer);
  ok.draw();
}

}  // namespace bridge

}  // namespace

LAB_EXAMPLE(
    "Bridge",
    "separate abstraction from implementation to avoid M x N subclasses") {
  problem::run();
  bridge::run();
}
