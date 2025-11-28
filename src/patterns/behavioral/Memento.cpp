// Memento is a behavioral design pattern that lets you save and restore the
// previous state of an object without violating encapsulation, captures and
// externalizes an object's internal state Appicability:
// (*)    when you want to produce snapshots of the object’s state to be able to
// restore a previous state of the object.
// (**)   when direct access to the object’s fields/getters/setters violates its
// encapsulation. UML: docs/uml/patterns_behavioral_memento.drawio.svg

#include <cstdlib>  // for std::rand, std::srand
#include <ctime>    // for std::time
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
namespace {
namespace Memento {
/**
 * Memento interface provides a way to retrieve the memento's metadata, such as
 * creation date or name. However, it doesn't expose the Originator's state.
 */
class IMemento {
 public:
  virtual ~IMemento() = default;

  virtual std::string getName() const = 0;
  virtual std::string getDate() const = 0;
  virtual std::string getState() const = 0;
};

/**
 * Concrete Memento contains the infrastructure for storing the Originator's
 * state.
 */
class ConcreteMemento : public IMemento {
 private:
  std::string m_state;
  std::string m_date;
  std::string m_name;

 public:
  explicit ConcreteMemento(const std::string& state) : m_state{state} {
    // Get current time
    std::time_t now = std::time(nullptr);
    std::tm* t = std::localtime(&now);

    // Format date as YYYYMMDD_HHMMSS
    std::stringstream date_ss;
    date_ss << std::put_time(t, "%Y%m%d_%H%M%S");
    m_date = date_ss.str();

    // Append a random number for uniqueness
    int rand_num = std::rand() % 10000;  // optional: limit size
    std::stringstream name_ss;
    name_ss << "mem_" << m_date << "_" << rand_num;
    m_name = name_ss.str();
  }

  std::string getName() const override { return m_name; };

  std::string getDate() const override { return this->m_date; };

  std::string getState() const override { return this->m_state; }
};

/**
 * Originator holds some important state that may change over time.
 * It also defines a method for saving the state inside a memento and another
 * method for restoring the state from it.
 */
class Originator {
 private:
  std::string m_state;

  // Simulate new state using rand
  static std::string generateRandomString(int len = 10) {
    // String literal concatenation
    const char alphaNum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    int strLen = sizeof(alphaNum) - 1;
    std::string ranStr;
    for (int i = 0; i < len; ++i) {
      ranStr += alphaNum[std::rand() % strLen];
    }
    return ranStr;
  }

 public:
  explicit Originator(const std::string& state) : m_state{state} {
    std::cout << "[O]Initial state is: " << this->m_state << "\n";
  }

  void operation() {
    std::cout << "[O]Doing something important.\n";
    this->m_state = this->generateRandomString(30);
    std::cout << "[O]The state has changed to: " << this->m_state << "\n";
  }

  //  Save the current state inside a memento.
  IMemento* save() { return new ConcreteMemento(this->m_state); }

  // Restores the Originator's state from a memento object.
  void restore(IMemento* mem) {
    this->m_state = mem->getState();
    std::cout << "[O]The state has restored to: " << this->m_state << "\n";

    delete mem;
  }
};

/**
 * The Caretaker doesn't depend on the Concrete Memento class. Therefore, it
 * doesn't have access to the originator's state, stored inside the memento. It
 * works with all mementos via the base Memento interface.
 */
class CareTaker {
 private:
  std::vector<IMemento*> m_mementos;
  Originator* m_originator;

 public:
  explicit CareTaker(Originator* const org) : m_originator{org} {}
  ~CareTaker() {
    for (IMemento* m : m_mementos) {
      delete m;
    }
  }

  void backup() {
    std::cout << "[C]Saving Originator's state...\n";
    this->m_mementos.push_back(this->m_originator->save());
  }

  void undo() {
    if (this->m_mementos.size() != 0) {
      IMemento* mem = m_mementos.back();
      this->m_mementos.pop_back();

      std::cout << "[C]Restoring state to: " << mem->getName() << "\n";
      this->m_originator->restore(mem);
    }
  }

  void history() const {
    std::cout << "[C]The list of mementos:\n";
    for (const IMemento* m : m_mementos) {
      std::cout << "\t" << m->getName() << "\n";
    }
  }
};

namespace Client {
void clientCode(Originator* const org) {
  CareTaker* careTaker = new CareTaker(org);

  // 1
  careTaker->backup();
  org->operation();

  // 2
  careTaker->backup();
  org->operation();

  // 3
  careTaker->backup();
  org->operation();

  careTaker->history();
  careTaker->undo();
  careTaker->undo();
  careTaker->history();
  careTaker->undo();

  // [P] The previous state can’t be restored directly because m_state is
  // private. We need a Memento to save and recover internal state safely.
}
}  // namespace Client

void run() {
  // Gen seed
  std::srand(static_cast<unsigned int>(std::time(NULL)));

  Originator* origin = new Originator("Hello World");
  Client::clientCode(origin);

  delete origin;
}
}  // namespace Memento
}  // namespace

struct MementoAutoRunner {
  MementoAutoRunner() {
    std::cout << "\n--- Memento Pattern Example ---\n";
    Memento::run();
  }
};

static MementoAutoRunner instance;