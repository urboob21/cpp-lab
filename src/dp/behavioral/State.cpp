// -----------------------------------------------------------------------------
// State (behavioral pattern)
//
// Lets an object change its behavior when its internal state changes - it
// looks as if the object changed its class. Each state is a class; the context
// delegates the state-specific work to the current state object.
//
// Problem: a class full of `switch (state_)` blocks in every method, where
// adding a state means touching all of them.
// Solution: one class per state; transitions are explicit.
//
// Use it when:
//   - an object behaves differently depending on its state, with many states
//   - state-specific code changes often
//   - there are massive conditionals on the current state
//
// Participants:
//   Context        Device - holds the current state, delegates to it
//   State          DeviceState interface
//   ConcreteState  IdleState, ProcessingState, ErrorState
//
// Implementation note: a state that replaces itself in the middle of its own
// handle() would destroy the object that is still running. Here the context
// applies the requested transition AFTER handle() returns.
//
// UML: docs/uml/dp/behavioral_state.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Device;

/// State interface.
class DeviceState {
 public:
  virtual ~DeviceState() = default;
  virtual std::string name() const = 0;
  virtual void handle(Device& device) = 0;
};

/// Context.
class Device {
 public:
  explicit Device(std::unique_ptr<DeviceState> initial)
      : state_{std::move(initial)} {}

  /// One tick of work: delegates to the current state.
  void tick() {
    state_->handle(*this);
    if (next_state_) {
      LOG_S("    transition " << state_->name() << " -> "
                              << next_state_->name());
      state_ = std::move(next_state_);  // safe: handle() has already returned
    }
  }

  /// Called by states to request a transition.
  void transitionTo(std::unique_ptr<DeviceState> next) {
    next_state_ = std::move(next);
  }

  int startJob() { return ++jobs_; }

 private:
  std::unique_ptr<DeviceState> state_;
  std::unique_ptr<DeviceState> next_state_;
  int jobs_{0};
};

class IdleState : public DeviceState {
 public:
  std::string name() const override { return "Idle"; }
  void handle(Device& device) override;
};

class ProcessingState : public DeviceState {
 public:
  std::string name() const override { return "Processing"; }
  void handle(Device& device) override;
};

class ErrorState : public DeviceState {
 public:
  std::string name() const override { return "Error"; }
  void handle(Device& device) override {
    LOG("  [Error] resetting the device");
    device.transitionTo(std::make_unique<IdleState>());
  }
};

void IdleState::handle(Device& device) {
  LOG("  [Idle] new job arrived");
  device.transitionTo(std::make_unique<ProcessingState>());
}

void ProcessingState::handle(Device& device) {
  const int job = device.startJob();
  const bool failed = job % 3 == 0;  // every third job fails in this simulation
  LOG_S("  [Processing] job " << job << (failed ? " failed" : " done"));
  if (failed) {
    device.transitionTo(std::make_unique<ErrorState>());
  } else {
    device.transitionTo(std::make_unique<IdleState>());
  }
}

void run() {
  LOG_SECTION("A device moving through Idle -> Processing -> Error states");
  Device device{std::make_unique<IdleState>()};
  for (int tick = 0; tick < 8; ++tick) {
    device.tick();
  }
}

}  // namespace

LAB_EXAMPLE("State",
            "an object changes behavior with its state: one class per state") {
  run();
}
