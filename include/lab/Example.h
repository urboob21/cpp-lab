#pragma once

// -----------------------------------------------------------------------------
// LAB_EXAMPLE: declare a runnable example in a single line.
//
//   #include "lab/Example.h"
//   #include "lab/Logger.h"
//
//   namespace {
//   void demo() { LOG("Hello from the lab"); }
//   }  // namespace
//
//   LAB_EXAMPLE("Hello", "Prints a greeting") { demo(); }
//
// - The menu group comes from the file location: an example defined in
//   src/core/smart_pointer/Weak.cpp is listed as "core/smart_pointer/<name>".
// - The name must be unique inside its folder and contain no spaces or '/'.
// - Pass lab::kInteractive as a third argument for examples that read stdin,
//   wait for a network peer or never return. They are skipped by --run-all
//   and by the ctest smoke tests:
//
//   LAB_EXAMPLE("EchoServer", "TCP echo server on port 8080", lab::kInteractive)
//   { ... }
//
// How it works: the macro declares a file-local function, registers its
// address in lab::Registry from a static initializer (which runs before
// main()), and then lets the braces that follow become the function body.
// See docs/adding-examples.md.
// -----------------------------------------------------------------------------

#include "lab/Registry.h"  // IWYU pragma: export

namespace lab::detail {

// Turn the optional third macro argument into flags:
// flags() for LAB_EXAMPLE(name, description), flags(value) otherwise.
constexpr unsigned flags() noexcept {
  return kNone;
}
constexpr unsigned flags(unsigned value) noexcept {
  return value;
}

}  // namespace lab::detail

#define LAB_DETAIL_CONCAT_IMPL(a, b) a##b
#define LAB_DETAIL_CONCAT(a, b) LAB_DETAIL_CONCAT_IMPL(a, b)

#define LAB_EXAMPLE(name, description, ...)                               \
  LAB_DETAIL_EXAMPLE(LAB_DETAIL_CONCAT(lab_example_, __COUNTER__), name, \
                     description, ::lab::detail::flags(__VA_ARGS__))

#define LAB_DETAIL_EXAMPLE(function, name, description, flags)                    \
  static void function();                                                         \
  [[maybe_unused]] static const bool LAB_DETAIL_CONCAT(function, _registered) =   \
      ::lab::Registry::instance().add(__FILE__, name, description, &function, flags); \
  static void function()
