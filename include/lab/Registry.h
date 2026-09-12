#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace lab {

/// Optional traits of an example. Combine them with `|`.
enum ExampleFlags : unsigned {
  kNone = 0U,
  /// The example needs a human or another process: it reads stdin, waits for a
  /// network peer, or never returns. `--run-all` and the ctest smoke tests
  /// skip these examples.
  kInteractive = 1U << 0U,
};

using ExampleFunction = void (*)();

/// Metadata and entry point of one runnable example.
struct Example {
  std::string id;           ///< "<group>/<name>", e.g. "core/smart_pointer/Weak"
  std::string group;        ///< source directory below src/, e.g. "core/smart_pointer"
  std::string name;         ///< short name shown in the menu, e.g. "Weak"
  std::string description;  ///< one-line summary
  std::string source;       ///< path relative to the repository root
  ExampleFunction run{nullptr};
  unsigned flags{kNone};

  bool isInteractive() const { return (flags & kInteractive) != 0U; }
};

/// Collection of all examples.
///
/// Examples register themselves through LAB_EXAMPLE (see lab/Example.h) while
/// static objects are initialized, i.e. before main() runs. The group of an
/// example is derived from the directory of its source file, so the menu
/// always mirrors the folder structure below src/.
class Registry {
 public:
  /// The process-wide registry used by LAB_EXAMPLE.
  static Registry& instance();

  /// Registers an example. `file` is the __FILE__ of the example.
  /// Returns false (and records an error) for invalid or duplicate examples.
  bool add(std::string_view file, std::string_view name,
           std::string_view description, ExampleFunction run,
           unsigned flags = kNone);

  /// All registered examples, sorted case-insensitively by id.
  const std::vector<Example>& examples() const { return examples_; }

  /// Looks up an example by id. Exact matches win over case-insensitive ones.
  const Example* find(std::string_view id) const;

  /// Examples whose id or description contains `filter` (case-insensitive).
  /// An empty filter matches every example.
  std::vector<const Example*> match(std::string_view filter) const;

  /// Problems found while registering (duplicate ids, invalid names, ...).
  const std::vector<std::string>& errors() const { return errors_; }

 private:
  std::vector<Example> examples_;
  std::vector<std::string> errors_;
};

namespace detail {

/// Makes `file` relative to `source_root`:
/// "/repo/src/core/Weak.cpp" -> "src/core/Weak.cpp".
std::string relativeSourcePath(std::string_view file,
                               std::string_view source_root);

/// Derives the menu group from a relative source path:
/// "src/core/smart_pointer/Weak.cpp" -> "core/smart_pointer".
std::string groupFromSourcePath(std::string_view relative_path);

}  // namespace detail
}  // namespace lab
