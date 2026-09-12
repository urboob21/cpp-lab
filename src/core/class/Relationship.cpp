// -----------------------------------------------------------------------------
// Relationships between classes (see README.md in this folder for UML)
//
//   Composition  "part-of"    : the whole owns the part; they live and die together
//   Aggregation  "has-a"      : the whole uses a part it does not own
//   Association  "uses-a"     : two otherwise independent objects collaborate
//   Dependency   "depends-on" : a class uses another only temporarily
//   Container    "holds many" : stores values (composition) or pointers (aggregation)
//   Nested class               : a type defined inside another type
//
// Reference: https://www.learncpp.com/cpp-tutorial/object-relationships/
// -----------------------------------------------------------------------------

#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace composition {

class Engine {
 public:
  Engine() { LOG("  Engine created"); }
  ~Engine() { LOG("  Engine destroyed"); }
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};

class Car {
 public:
  Car() { LOG("  Car created"); }
  ~Car() { LOG("  Car destroyed"); }
  Car(const Car&) = delete;
  Car& operator=(const Car&) = delete;

 private:
  Engine engine_;  // composition: created and destroyed together with the Car
};

void run() {
  LOG_SECTION("Composition (part-of)");
  const Car car;
  LOG("-- leaving scope: the Car takes its Engine with it --");
}

}  // namespace composition

namespace aggregation {

class Teacher {
 public:
  explicit Teacher(std::string name) : name_{std::move(name)} {
    LOG_S("  Teacher " << name_ << " created");
  }
  ~Teacher() { LOG_S("  Teacher " << name_ << " destroyed"); }
  Teacher(const Teacher&) = delete;
  Teacher& operator=(const Teacher&) = delete;

  const std::string& name() const { return name_; }

 private:
  std::string name_;
};

class Department {
 public:
  explicit Department(std::string name, const Teacher& teacher)
      : name_{std::move(name)}, teacher_{&teacher} {
    LOG_S("  Department " << name_ << " created with " << teacher_->name());
  }
  ~Department() { LOG_S("  Department " << name_ << " destroyed"); }
  Department(const Department&) = delete;
  Department& operator=(const Department&) = delete;

 private:
  std::string name_;
  const Teacher* teacher_;  // aggregation: points to a Teacher it does NOT own
};

void run() {
  LOG_SECTION("Aggregation (has-a)");
  const Teacher teacher{"Mr. A"};
  {
    const Department math{"Math", teacher};
    const Department physics{"Physics",
                             teacher};  // one teacher, two departments
    LOG("-- departments close --");
  }
  LOG("-- the teacher still exists --");
}

}  // namespace aggregation

namespace association {

class Patient {
 public:
  explicit Patient(std::string name) : name_{std::move(name)} {}
  const std::string& name() const { return name_; }

 private:
  std::string name_;
};

class Doctor {
 public:
  // The Doctor neither owns nor stores the Patient; they only interact.
  void treat(const Patient& patient) const {
    LOG_S("  Treating " << patient.name());
  }
};

void run() {
  LOG_SECTION("Association (uses-a)");
  const Doctor doctor;
  const Patient alice{"Alice"};
  const Patient bob{"Bob"};
  doctor.treat(alice);
  doctor.treat(bob);
}

}  // namespace association

namespace dependency {

class FileLogger {
 public:
  void write(const std::string& message) const {
    LOG_S("  [file] " << message);
  }
};

class Car {
 public:
  void start() const {
    const FileLogger logger;  // dependency: only needed during this call
    logger.write("Car started");
  }
};

void run() {
  LOG_SECTION("Dependency (depends-on)");
  const Car car;
  car.start();
}

}  // namespace dependency

namespace container {

class Library {
 public:
  void addBook(std::string title) { books_.push_back(std::move(title)); }
  std::size_t size() const { return books_.size(); }

 private:
  std::vector<std::string> books_;  // stores the values themselves
};

class School {
 public:
  void hire(const aggregation::Teacher& teacher) {
    teachers_.push_back(&teacher);
  }
  void printStaff() const {
    for (const aggregation::Teacher* teacher : teachers_) {
      LOG_S("  staff: " << teacher->name());
    }
  }

 private:
  std::vector<const aggregation::Teacher*>
      teachers_;  // stores non-owning pointers
};

void run() {
  LOG_SECTION("Container");
  Library library;
  library.addBook("The C++ Programming Language");
  library.addBook("Effective Modern C++");
  LOG_S("  library holds " << library.size() << " books (by value)");

  const aggregation::Teacher teacher_b{"Ms. B"};
  const aggregation::Teacher teacher_c{"Mr. C"};
  School school;
  school.hire(teacher_b);
  school.hire(teacher_c);
  school.printStaff();
}

}  // namespace container

namespace nested {

class Car {
 public:
  /// Nested class: a type that only makes sense in the context of Car.
  class Engine {
   public:
    void start() const { LOG("  Car::Engine::start"); }
  };

  void drive() const {
    engine_.start();
    LOG("  Car::drive");
  }

 private:
  Engine engine_;
};

void run() {
  LOG_SECTION("Nested (inner) class");
  const Car car;
  car.drive();
  const Car::Engine spare;  // usable from outside because Engine is public
  spare.start();
}

}  // namespace nested

}  // namespace

LAB_EXAMPLE("Relationship",
            "composition, aggregation, association, dependency, containers, "
            "nested classes") {
  composition::run();
  aggregation::run();
  association::run();
  dependency::run();
  container::run();
  nested::run();
}
