// -----------------------------------------------------------------------------
// Observer (behavioral pattern)
//
// Defines a subscription mechanism: a subject (publisher) notifies all its
// observers (subscribers) when an event happens, without knowing their
// concrete types.
//
// Use it when:
//   - changes in one object must update others, and the set of interested
//     objects is not known in advance or changes at run time
//   - some objects only need to listen for a limited time
//
// Real world: GUI events, model -> view updates (MVC, see src/ap), signals and
// slots, event buses.
//
// Participants:
//   Subject           Button - attach / detach / notify
//   Observer          ClickListener interface - onEvent()
//   ConcreteObserver  AnalyticsListener, SoundListener
//
// Pitfall: the subject stores pointers to observers. An observer destroyed
// while still attached leaves a dangling pointer - always detach first, or use
// subscriptions that detach automatically (RAII).
//
// UML: docs/uml/dp/behavioral_observer.drawio.svg
// -----------------------------------------------------------------------------

#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

enum class Event { kPressed, kReleased };

std::string toString(Event event) {
  return event == Event::kPressed ? "pressed" : "released";
}

namespace classic {

/// Observer interface.
class Listener {
 public:
  virtual ~Listener() = default;
  virtual void onEvent(const std::string& source, Event event) = 0;
};

/// Subject.
class Button {
 public:
  explicit Button(std::string name) : name_{std::move(name)} {}

  void attach(Listener& listener) { listeners_.push_back(&listener); }
  void detach(Listener& listener) { std::erase(listeners_, &listener); }

  void press() { notify(Event::kPressed); }
  void release() { notify(Event::kReleased); }

 private:
  void notify(Event event) {
    LOG_S("  " << name_ << " " << toString(event) << ", notifying "
               << listeners_.size() << " listener(s)");
    for (Listener* listener : listeners_) {
      listener->onEvent(name_, event);
    }
  }

  std::string name_;
  std::vector<Listener*> listeners_;  // non-owning
};

class AnalyticsListener : public Listener {
 public:
  void onEvent(const std::string& source, Event event) override {
    ++events_;
    LOG_S("    [analytics] recorded " << source << " " << toString(event)
                                      << " (total " << events_ << ")");
  }

 private:
  int events_{0};
};

class SoundListener : public Listener {
 public:
  void onEvent(const std::string& /*source*/, Event event) override {
    if (event == Event::kPressed) {
      LOG("    [sound] click!");
    }
  }
};

void run() {
  LOG_SECTION("Classic Observer with an interface");
  Button save{"Save button"};
  AnalyticsListener analytics;
  SoundListener sound;

  save.attach(analytics);
  save.attach(sound);
  save.press();
  save.release();

  LOG("  -- detaching the sound listener --");
  save.detach(sound);
  save.press();
}

}  // namespace classic

namespace callbacks {

/// A lighter alternative: observers are std::function callbacks.
class Button {
 public:
  using Callback = std::function<void(Event)>;

  /// Returns an id that can be used to unsubscribe.
  int subscribe(Callback callback) {
    subscribers_.emplace_back(next_id_, std::move(callback));
    return next_id_++;
  }

  void unsubscribe(int id) {
    std::erase_if(subscribers_,
                  [id](const auto& entry) { return entry.first == id; });
  }

  void press() const {
    for (const auto& [id, callback] : subscribers_) {
      callback(Event::kPressed);
    }
  }

 private:
  std::vector<std::pair<int, Callback>> subscribers_;
  int next_id_{0};
};

void run() {
  LOG_SECTION("Observers as std::function callbacks");
  Button button;
  int clicks = 0;
  const int counter = button.subscribe([&clicks](Event) { ++clicks; });
  button.subscribe([](Event event) {
    LOG_S("    lambda observer got " << toString(event));
  });

  button.press();
  button.press();
  button.unsubscribe(counter);
  button.press();
  LOG_S("  counter observer saw "
        << clicks << " clicks (it unsubscribed before the third)");
}

}  // namespace callbacks

}  // namespace

LAB_EXAMPLE(
    "Observer",
    "publish/subscribe with listener interfaces or std::function callbacks") {
  classic::run();
  callbacks::run();
}
