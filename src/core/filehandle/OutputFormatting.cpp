// -----------------------------------------------------------------------------
// Output formatting with <iomanip> and std::format
//
// Two ways to change how a stream formats values:
//   - flags        : stream.setf(std::ios::showpos), unsetf(...)
//   - manipulators : stream << std::hex << std::setw(8) << std::setfill('0')
//
// Most settings are STICKY (they stay until changed); std::setw only applies
// to the next output. Save/restore the state with copyfmt when a function
// must not leave its settings behind.
//
// C++20 std::format("{:>8.2f}", x) formats without touching any stream state.
//
// This example formats into std::ostringstream objects, so the settings do
// not leak into the rest of the program.
//
// Reference: https://en.cppreference.com/w/cpp/io/manip
// -----------------------------------------------------------------------------

#include <format>
#include <iomanip>
#include <sstream>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void flags() {
  LOG_SECTION("Flags: boolalpha, showpos, uppercase");
  std::ostringstream out;

  out << true << ' ' << false;
  LOG_S("default        : " << out.str());

  out.str("");
  out.setf(std::ios::boolalpha);
  out << true << ' ' << false;
  LOG_S("boolalpha      : " << out.str());

  out.str("");
  out.setf(std::ios::showpos);
  out << 5 << ' ' << -5;
  LOG_S("showpos        : " << out.str());
  out.unsetf(std::ios::showpos);

  out.str("");
  out.setf(std::ios::uppercase);
  out << 12345678.9 << ' ' << std::hex << 255;
  LOG_S("uppercase      : " << out.str());
}

void numberBases() {
  LOG_SECTION("Number bases");
  std::ostringstream out;
  out << std::showbase << "dec " << std::dec << 255 << ", hex " << std::hex
      << 255 << ", oct " << std::oct << 255;
  LOG(out.str());
}

void floatingPoint() {
  LOG_SECTION("Floating point: precision, fixed, scientific");
  const double value = 123.456789;
  std::ostringstream out;

  out << value << " | " << std::setprecision(4) << value;
  LOG_S("default, setprecision(4) : " << out.str() << "  (significant digits)");

  out.str("");
  out << std::fixed << std::setprecision(2) << value;
  LOG_S("fixed + setprecision(2)  : " << out.str()
                                      << "  (digits after the point)");

  out.str("");
  out << std::scientific << std::setprecision(3) << value;
  LOG_S("scientific               : " << out.str());
}

void widthAndAlignment() {
  LOG_SECTION("Width, alignment and fill");
  std::ostringstream out;
  out << '|' << std::setw(10) << -12345 << "|  right (default)\n"
      << '|' << std::setw(10) << std::left << -12345 << "|  left\n"
      << '|' << std::setw(10) << std::internal << -12345 << "|  internal\n"
      << '|' << std::setfill('*') << std::setw(10) << std::right << -12345
      << "|  setfill('*')\n"
      << '|' << -12345 << "|  setw applies to one output only";
  LOG(out.str());

  LOG_SECTION("Saving and restoring the stream state");
  std::ostringstream stream;
  std::ios saved(nullptr);
  saved.copyfmt(stream);  // remember the current settings
  stream << std::hex << std::uppercase << 3054;
  stream.copyfmt(saved);  // back to the defaults
  stream << ' ' << 3054;
  LOG_S("hex then restored: " << stream.str());
}

void stdFormat() {
  LOG_SECTION("C++20 std::format");
  LOG(std::format("|{:>10}|{:<10}|{:^10}|", "right", "left", "center"));
  LOG(std::format("pi = {:.3f}, hex = {:#x}, padded = {:08.2f}", 3.14159, 255,
                  3.14159));
  LOG(std::format("{0} + {0} = {1}", 21, 42));  // positional arguments
}

}  // namespace

LAB_EXAMPLE("OutputFormatting",
            "iomanip flags and manipulators, sticky state, std::format") {
  flags();
  numberBases();
  floatingPoint();
  widthAndAlignment();
  stdFormat();
}
