// Observer is a behavioral design pattern that lets you define a subscription
// mechanism to notify multiple objects about any events that happen to the
// object they’re observing. Usage examples: The most popular usage of the
// Observer pattern in C++ code is facilitating communications between GUI
// components of an app. The synonym of the Observer is the `Controller` part of
// MVC pattern. Appicability:
// (*)   when changes to the state of one object may require changing other
// objects, and the actual set of objects is unknown beforehand or changes
// dynamically.
// (**)   when some objects in your app must observe others, but only for a
// limited time or in specific cases.

// UML: docs/uml/patterns_behavioral_observer.drawio.svg

#include <iostream>
#include <list>
#include <string>

namespace {
namespace Observer {

enum class Event {
  CREATE = 0,
  READ,
  UPDATE,
  DELETE,
};

static inline const char* getEventName(const Event& e) {
  switch (e) {
    case Event::CREATE:
      return "CREATE";
    case Event::READ:
      return "READ";
    case Event::UPDATE:
      return "UPDATE";
    case Event::DELETE:
      return "DELETE";
  }
  return "invalid_event";
}

/**
 * IObserver aka Subscriber
 * The Subscriber interface declares the notification interface.
 * In most cases, it consists of a single update method.
 * The method may have several parameters that let the publisher pass some event
 * details along with the update. E.g. Event Listen to UI events
 */
class IListenerObserver {
 public:
  virtual ~IListenerObserver() = default;

  // update
  virtual void update(const Event e) = 0;
};

/**
 * Subject aka Publisher
 * The Publisher issues events of interest to other objects.
 * These events occur when the publisher changes its state or executes some
 * behaviors. Publishers contain a subscription infrastructure that lets new
 * subscribers join and current subscribers leave the list.
 *
 * E.g Widget dispatches click events to observers
 */
class IWidgetSubject {
 public:
  virtual ~IWidgetSubject(){};
  // addListener
  virtual void attach(IListenerObserver* observer) = 0;
  // removeLister
  virtual void detach(IListenerObserver* observer) = 0;
  // e.g.click
  virtual void notify(const Event& e) = 0;
};

class ButtonConcreteSubject : public IWidgetSubject {
 private:
  std::list<IListenerObserver*> m_listeners;

 public:
  void attach(IListenerObserver* observer) override {
    m_listeners.push_back(observer);
  }

  void detach(IListenerObserver* observer) override {
    m_listeners.remove(observer);
  }

  void notify(const Event& e) override {
    std::cout << "[Subject] notify event-" << getEventName(e) << "\n";
    for (IListenerObserver* o : m_listeners) {
      o->update(e);
    }
  }
};

class AbstractListenerObserver : public IListenerObserver {
 private:
  int m_num;
  inline static int num_observers = 0;

 protected:
  void log(const Event& e) const {
    std::cout << "\t-id:" << m_num << "-event:" << getEventName(e) << "\n";
  }

 public:
  explicit AbstractListenerObserver() { m_num = ++num_observers; }
};

/**
 * Concrete Subscribers perform some actions in response to notifications issued
 * by the publisher. All of these classes must implement the same interface so
 * the publisher isn’t coupled to concrete classes.
 */
class ConcreteListenerObserverA : public AbstractListenerObserver {
 private:
  static const inline char* type = "A-type";

 public:
  void update(const Event e) override {
    std::cout << "\tListener: " << type;
    log(e);
  }
};

class ConcreteListenerObserverB : public AbstractListenerObserver {
 private:
  static const inline char* type = "B-type";

 public:
  void update(const Event e) override {
    std::cout << "\tListener: " << type;
    log(e);
  }
};

/**
 * The Client creates publisher and subscriber objects separately
 * and then registers subscribers for publisher updates.
 */
namespace Client {
void clientCode(IWidgetSubject* const s) {
  s->notify(Event::UPDATE);
}
}  // namespace Client

void run() {
  IWidgetSubject* btn = new ButtonConcreteSubject();

  IListenerObserver* listener_1 = new ConcreteListenerObserverA();
  IListenerObserver* listener_2 = new ConcreteListenerObserverA();
  IListenerObserver* listener_3 = new ConcreteListenerObserverA();
  IListenerObserver* listener_4 = new ConcreteListenerObserverB();

  btn->attach(listener_1);
  btn->attach(listener_2);
  btn->attach(listener_3);
  btn->attach(listener_4);
  Client::clientCode(btn);

  std::cout << "Remove listener2\n";
  btn->detach(listener_2);
  Client::clientCode(btn);

  delete btn;
  delete listener_1;
  delete listener_2;
  delete listener_3;
  delete listener_4;
}

}  // namespace Observer
}  // namespace

struct ObserverAutoRunner {
  ObserverAutoRunner() {
    std::cout << "\n--- Observer Pattern Example ---\n";
    Observer::run();
  }
};

static ObserverAutoRunner instance;