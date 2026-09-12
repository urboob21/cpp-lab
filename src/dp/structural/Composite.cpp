// -----------------------------------------------------------------------------
// Composite (structural pattern)
//
// Composes objects into tree structures and lets clients treat individual
// objects (leaves) and groups of objects (composites) the same way.
//
// Problem: a file browser with separate File and Folder classes. Every
// operation (size, print, search) needs `if file ... else if folder ...`
// branches, and each new node type touches all of them.
// Solution: File and Folder share one interface. A Folder implements the
// operations by delegating to its children, recursively.
//
// Use it when:
//   - you have a tree-like structure (file systems, GUIs, org charts, scenes)
//   - clients should not care whether they deal with a leaf or a group
//
// Participants:
//   Component  Node - common interface
//   Leaf       File, Shortcut - no children
//   Composite  Folder - owns children and delegates to them
//
// UML: docs/uml/dp/structural_composite.drawio.svg
// -----------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Component.
class Node {
 public:
  explicit Node(std::string name) : name_{std::move(name)} {}
  virtual ~Node() = default;

  const std::string& name() const { return name_; }

  virtual std::size_t sizeBytes() const = 0;
  virtual void print(int depth) const = 0;

 protected:
  static std::string indent(int depth) {
    return std::string(static_cast<std::size_t>(depth) * 2, ' ');
  }

 private:
  std::string name_;
};

/// Leaf.
class File : public Node {
 public:
  File(std::string name, std::size_t bytes)
      : Node{std::move(name)}, bytes_{bytes} {}

  std::size_t sizeBytes() const override { return bytes_; }
  void print(int depth) const override {
    LOG_S(indent(depth) << "- " << name() << " (" << bytes_ << " B)");
  }

 private:
  std::size_t bytes_;
};

/// Another leaf: refers to a path, takes (almost) no space itself.
class Shortcut : public Node {
 public:
  Shortcut(std::string name, std::string target)
      : Node{std::move(name)}, target_{std::move(target)} {}

  std::size_t sizeBytes() const override { return 1; }
  void print(int depth) const override {
    LOG_S(indent(depth) << "~ " << name() << " -> " << target_);
  }

 private:
  std::string target_;
};

/// Composite: owns its children and delegates to them.
class Folder : public Node {
 public:
  using Node::Node;

  /// Adds a child and returns a reference to it for convenient tree building.
  template <typename T, typename... Args>
  T& add(Args&&... args) {
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    T& reference = *child;
    children_.push_back(std::move(child));
    return reference;
  }

  bool remove(const std::string& child_name) {
    return std::erase_if(children_, [&child_name](const auto& child) {
             return child->name() == child_name;
           }) > 0;
  }

  std::size_t sizeBytes() const override {
    std::size_t total = 0;
    for (const auto& child : children_) {
      total += child->sizeBytes();  // leaf or folder - no need to know
    }
    return total;
  }

  void print(int depth) const override {
    LOG_S(indent(depth) << "+ " << name() << "/ (" << sizeBytes() << " B)");
    for (const auto& child : children_) {
      child->print(depth + 1);
    }
  }

 private:
  std::vector<std::unique_ptr<Node>>
      children_;  // destroyed recursively with the folder
};

/// Client code: works with any Node.
void report(const Node& node) {
  LOG_S("  " << node.name() << " uses " << node.sizeBytes() << " bytes");
}

void run() {
  LOG_SECTION("Building a tree");
  Folder root{"root"};
  root.add<File>("readme.md", 1200);
  Folder& src = root.add<Folder>("src");
  src.add<File>("main.cpp", 5400);
  Folder& lab = src.add<Folder>("lab");
  lab.add<File>("Registry.cpp", 4100);
  lab.add<File>("Menu.cpp", 6300);
  root.add<Shortcut>("latest-log", "/var/log/cpplab.log");
  root.print(1);

  LOG_SECTION("The client treats leaves and composites alike");
  report(root);
  report(src);
  report(lab);

  LOG_SECTION("Removing a subtree");
  src.remove("lab");  // unique_ptr frees the whole subtree
  root.print(1);
}

}  // namespace

LAB_EXAMPLE("Composite",
            "tree structures where leaves and groups share one interface") {
  run();
}
