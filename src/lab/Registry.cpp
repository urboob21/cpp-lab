#include "lab/Registry.h"

#include <algorithm>
#include <cctype>
#include <utility>

// Absolute path of the repository, injected by CMake. Used to turn __FILE__
// into a repository-relative path.
#ifndef CPPLAB_SOURCE_DIR
#define CPPLAB_SOURCE_DIR ""
#endif

namespace lab {
namespace {

std::string toLower(std::string_view text) {
  std::string lower(text);
  std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return lower;
}

bool lessIgnoreCase(std::string_view lhs, std::string_view rhs) {
  return std::lexicographical_compare(
      lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
      [](unsigned char a, unsigned char b) {
        return std::tolower(a) < std::tolower(b);
      });
}

bool equalIgnoreCase(std::string_view lhs, std::string_view rhs) {
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                    [](unsigned char a, unsigned char b) {
                      return std::tolower(a) == std::tolower(b);
                    });
}

bool isValidName(std::string_view name) {
  return !name.empty() &&
         std::none_of(name.begin(), name.end(), [](unsigned char c) {
           return c == '/' || std::isspace(c) != 0;
         });
}

}  // namespace

namespace detail {

std::string relativeSourcePath(std::string_view file,
                               std::string_view source_root) {
  std::string path(file);
  std::replace(path.begin(), path.end(), '\\', '/');

  std::string root(source_root);
  std::replace(root.begin(), root.end(), '\\', '/');
  if (!root.empty()) {
    if (root.back() != '/') {
      root.push_back('/');
    }
    if (path.rfind(root, 0) == 0) {
      return path.substr(root.size());
    }
  }

  // Fallback when __FILE__ is not below the root (e.g. relative paths):
  // keep everything from the last "src/" directory on.
  if (path.rfind("src/", 0) == 0) {
    return path;
  }
  if (const auto pos = path.rfind("/src/"); pos != std::string::npos) {
    return path.substr(pos + 1);
  }
  return path;
}

std::string groupFromSourcePath(std::string_view relative_path) {
  std::string_view path = relative_path;
  if (path.rfind("src/", 0) == 0) {
    path.remove_prefix(4);
  }
  const auto slash = path.rfind('/');
  if (slash == std::string_view::npos || slash == 0) {
    return "misc";
  }
  return std::string(path.substr(0, slash));
}

}  // namespace detail

Registry& Registry::instance() {
  // A function-local static is created on first use. This makes it safe to
  // call from other static initializers (the "static initialization order
  // fiasco" cannot happen).
  static Registry registry;
  return registry;
}

bool Registry::add(std::string_view file, std::string_view name,
                   std::string_view description, ExampleFunction run,
                   unsigned flags) {
  Example example;
  example.source = detail::relativeSourcePath(file, CPPLAB_SOURCE_DIR);
  example.group = detail::groupFromSourcePath(example.source);
  example.name = std::string(name);
  example.id = example.group + "/" + example.name;
  example.description = std::string(description);
  example.run = run;
  example.flags = flags;

  if (!isValidName(name)) {
    errors_.push_back(example.source + ": invalid example name '" +
                      example.name + "' (use no spaces and no '/')");
    return false;
  }
  if (run == nullptr) {
    errors_.push_back(example.source + ": example '" + example.name +
                      "' has no function");
    return false;
  }

  const auto position = std::lower_bound(
      examples_.begin(), examples_.end(), example.id,
      [](const Example& existing, const std::string& id) {
        return lessIgnoreCase(existing.id, id);
      });
  if (position != examples_.end() &&
      equalIgnoreCase(position->id, example.id)) {
    errors_.push_back("duplicate example id '" + example.id + "' in " +
                      example.source + " (already defined in " +
                      position->source + ")");
    return false;
  }

  examples_.insert(position, std::move(example));
  return true;
}

const Example* Registry::find(std::string_view id) const {
  const Example* case_insensitive = nullptr;
  for (const Example& example : examples_) {
    if (example.id == id) {
      return &example;
    }
    if (case_insensitive == nullptr && equalIgnoreCase(example.id, id)) {
      case_insensitive = &example;
    }
  }
  return case_insensitive;
}

std::vector<const Example*> Registry::match(std::string_view filter) const {
  const std::string needle = toLower(filter);
  std::vector<const Example*> matches;
  for (const Example& example : examples_) {
    if (needle.empty() ||
        toLower(example.id).find(needle) != std::string::npos ||
        toLower(example.description).find(needle) != std::string::npos) {
      matches.push_back(&example);
    }
  }
  return matches;
}

}  // namespace lab
