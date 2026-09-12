// -----------------------------------------------------------------------------
// std::unique_ptr - exclusive ownership
//
//   auto p = std::make_unique<T>(args...);
//
//   - Exactly one unique_ptr owns the object; the object is deleted when that
//     unique_ptr is destroyed or reset (RAII). No manual delete, no leaks.
//   - It cannot be copied, only MOVED: ownership is transferred explicitly.
//   - Zero overhead compared to a raw pointer (with the default deleter).
//   - Function parameters express intent:
//       f(std::unique_ptr<T>)  f takes ownership (a "sink")
//       f(T&) / f(T*)          f only uses the object, ownership stays
//   - A custom deleter releases non-memory resources (files, handles).
//
// Reference: https://en.cppreference.com/w/cpp/memory/unique_ptr
// -----------------------------------------------------------------------------

#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Resource {
 public:
  explicit Resource(std::string name) : name_{std::move(name)} {
    LOG_S("  Resource(" << name_ << ") acquired");
  }
  ~Resource() { LOG_S("  ~Resource(" << name_ << ") released"); }
  Resource(const Resource&) = delete;
  Resource& operator=(const Resource&) = delete;

  const std::string& name() const { return name_; }

 private:
  std::string name_;
};

void basics() {
  LOG_SECTION("Ownership and moving");
  {
    auto first = std::make_unique<Resource>("A");
    // auto copy = first;  // error: unique_ptr cannot be copied
    auto second = std::move(first);  // ownership moves, nothing is copied
    LOG_S("  first is " << (first ? "set" : "empty") << ", second owns "
                        << second->name());
    LOG("  -- leaving scope --");
  }  // second is destroyed here and deletes Resource A

  LOG_SECTION("reset, release and get");
  auto owner = std::make_unique<Resource>("B");
  const Resource* observer = owner.get();  // non-owning raw pointer
  LOG_S("  get() -> " << observer->name());

  owner.reset(new Resource("C"));   // deletes B, now owns C
  Resource* raw = owner.release();  // gives up ownership WITHOUT deleting
  LOG_S("  after release(): owner is " << (owner ? "set" : "empty")
                                       << ", raw points to " << raw->name());
  delete raw;  // now it is our job - which is why release() is rarely used
}

void consume(std::unique_ptr<Resource> resource) {  // sink: takes ownership
  LOG_S("  consume() owns " << resource->name() << " now");
}  // deleted here

void inspect(
    const Resource& resource) {  // borrows, ownership stays with the caller
  LOG_S("  inspect() looks at " << resource.name());
}

std::unique_ptr<Resource> makeResource(const std::string& name) {
  return std::make_unique<Resource>(name);  // factory: returns ownership
}

void functions() {
  LOG_SECTION("Passing ownership through functions");
  auto resource = makeResource("D");
  inspect(*resource);
  consume(std::move(resource));
  LOG_S("  after consume(): resource is " << (resource ? "set" : "empty"));
}

void containersAndArrays() {
  LOG_SECTION("Containers and arrays");
  std::vector<std::unique_ptr<Resource>> resources;
  resources.push_back(std::make_unique<Resource>("E"));
  resources.push_back(std::make_unique<Resource>("F"));
  LOG_S("  vector holds " << resources.size() << " unique_ptrs");

  const auto numbers = std::make_unique<int[]>(3);  // value-initialized: 0 0 0
  numbers[1] = 42;
  LOG_S("  unique_ptr<int[]>: numbers[1] = "
        << numbers[1] << " (delete[] used automatically)");
  LOG("  -- clearing the vector --");
  resources.clear();
}

void customDeleter() {
  LOG_SECTION("Custom deleter for a C resource");
  auto close_file = [](std::FILE* file) {
    LOG("  deleter: std::fclose");
    std::fclose(file);
  };
  const std::unique_ptr<std::FILE, decltype(close_file)> file(std::tmpfile(),
                                                              close_file);
  if (file) {
    std::fputs("temporary data", file.get());
    LOG("  wrote to a temporary FILE*; it is closed automatically");
  }
}

}  // namespace

LAB_EXAMPLE(
    "Unique",
    "std::unique_ptr: ownership, moves, sinks, arrays, custom deleters") {
  basics();
  functions();
  containersAndArrays();
  customDeleter();
}
