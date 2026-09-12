// -----------------------------------------------------------------------------
// Colors in the terminal (ANSI escape codes)
//
//   "\033[<code>m" changes the style of the text that follows,
//   "\033[0m" resets everything.
//
//   30-37 foreground   40-47 background   90-97 bright foreground
//   1 bold   2 dim   3 italic   4 underline   7 reverse
//   Codes combine with ';', e.g. "\033[1;31m" is bold red.
//
// Only terminals understand these codes. When output goes to a file or a pipe
// (check with isatty) or NO_COLOR is set, print plain text - lab::Logger does.
//
// Reference: https://en.wikipedia.org/wiki/ANSI_escape_code
// -----------------------------------------------------------------------------

#include <array>
#include <iostream>
#include <string>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace ansi {
inline constexpr std::string_view kReset = "\033[0m";
}  // namespace ansi

struct Style {
  std::string_view name;
  std::string_view code;
};

constexpr std::array<Style, 8> kColors{{
    {"black", "30"},
    {"red", "31"},
    {"green", "32"},
    {"yellow", "33"},
    {"blue", "34"},
    {"magenta", "35"},
    {"cyan", "36"},
    {"white", "37"},
}};

constexpr std::array<Style, 5> kEffects{{
    {"bold", "1"},
    {"dim", "2"},
    {"italic", "3"},
    {"underline", "4"},
    {"reverse", "7"},
}};

std::string styled(std::string_view text, std::string_view code) {
  return "\033[" + std::string(code) + "m" + std::string(text) +
         std::string(ansi::kReset);
}

void run() {
  // Plain std::cout keeps the color table free of log prefixes.
  LOG_SECTION("Foreground colors: normal, bold and bright");
  for (const Style& color : kColors) {
    const std::string bold = "1;" + std::string(color.code);
    const std::string bright =
        std::to_string(std::stoi(std::string(color.code)) + 60);
    std::cout << "  " << styled(color.name, color.code) << "  "
              << styled(color.name, bold) << "  " << styled(color.name, bright)
              << '\n';
  }

  LOG_SECTION("Background colors");
  for (const Style& color : kColors) {
    const std::string background =
        std::to_string(std::stoi(std::string(color.code)) + 10);
    std::cout << "  "
              << styled("  " + std::string(color.name) + "  ", background)
              << '\n';
  }

  LOG_SECTION("Text effects (support varies by terminal)");
  for (const Style& effect : kEffects) {
    std::cout << "  " << styled(effect.name, effect.code) << '\n';
  }
  std::cout.flush();
}

}  // namespace

LAB_EXAMPLE("TerminalColor",
            "ANSI escape codes: colors, backgrounds and text effects") {
  run();
}
