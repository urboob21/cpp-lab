// -----------------------------------------------------------------------------
// Mediator (behavioral pattern)
//
// Reduces chaotic dependencies between objects: instead of talking to each
// other directly, components talk only to a mediator that coordinates them.
//
// Problem: with N aircraft that negotiate the runway directly, every aircraft
// must know every other aircraft (N * (N - 1) connections) and the landing
// rules are spread over all of them.
// Solution: a control tower (mediator) owns the rules; each aircraft only
// knows the tower.
//
// Use it when:
//   - classes are hard to change because they are tightly coupled to many others
//   - a component cannot be reused because it depends on too many others
//   - coordination logic is scattered over many classes
//
// Participants:
//   Mediator           ControlTower interface
//   ConcreteMediator   Tower - implements the coordination rules
//   Component          Aircraft - knows only the mediator interface
//
// Related: the Controller in MVC plays a mediator role (see src/ap).
// UML: docs/uml/dp/behavioral_mediator.drawio.svg
// -----------------------------------------------------------------------------

#include <deque>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Aircraft;

/// Mediator interface.
class ControlTower {
 public:
  virtual ~ControlTower() = default;
  virtual void requestLanding(Aircraft& aircraft) = 0;
  virtual void reportLanded(Aircraft& aircraft) = 0;
};

/// Component: communicates only through the tower.
class Aircraft {
 public:
  Aircraft(std::string callsign, ControlTower& tower)
      : callsign_{std::move(callsign)}, tower_{tower} {}

  const std::string& callsign() const { return callsign_; }

  void requestLanding() {
    LOG_S("  " << callsign_ << ": requesting landing");
    tower_.requestLanding(*this);
  }

  // Called by the tower.
  void clearToLand() {
    cleared_ = true;
    LOG_S("  " << callsign_ << ": cleared to land");
  }
  void hold() const { LOG_S("  " << callsign_ << ": holding pattern"); }

  void land() {
    if (!cleared_) {
      LOG_S("  " << callsign_ << ": cannot land without clearance");
      return;
    }
    LOG_S("  " << callsign_ << ": touchdown");
    cleared_ = false;
    tower_.reportLanded(*this);
  }

 private:
  std::string callsign_;
  ControlTower& tower_;
  bool cleared_{false};
};

/// Concrete mediator: one runway, first come first served.
class Tower : public ControlTower {
 public:
  void requestLanding(Aircraft& aircraft) override {
    if (runway_user_ == nullptr) {
      runway_user_ = &aircraft;
      aircraft.clearToLand();
    } else {
      waiting_.push_back(&aircraft);
      LOG_S("  [tower] runway busy with "
            << runway_user_->callsign() << ", " << aircraft.callsign()
            << " is number " << waiting_.size() << " in the queue");
      aircraft.hold();
    }
  }

  void reportLanded(Aircraft& aircraft) override {
    LOG_S("  [tower] " << aircraft.callsign() << " vacated the runway");
    runway_user_ = nullptr;
    if (!waiting_.empty()) {
      Aircraft* next = waiting_.front();
      waiting_.pop_front();
      runway_user_ = next;
      next->clearToLand();
    }
  }

 private:
  Aircraft* runway_user_{nullptr};
  std::deque<Aircraft*> waiting_;
};

void run() {
  LOG_SECTION("Three aircraft, one runway, coordinated by the tower");
  Tower tower;
  Aircraft alpha{"VN-101", tower};
  Aircraft bravo{"SQ-202", tower};
  Aircraft charlie{"JL-303", tower};

  alpha.requestLanding();
  bravo.requestLanding();
  charlie.requestLanding();
  charlie.land();  // not cleared yet

  alpha.land();  // the tower now clears bravo
  bravo.land();  // ...then charlie
  charlie.land();
}

}  // namespace

LAB_EXAMPLE("Mediator",
            "components coordinate through a mediator instead of each other") {
  run();
}
