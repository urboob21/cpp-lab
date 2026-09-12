#include "lab/Logger.h"

#include <unistd.h>  // isatty

#include <chrono>
#include <cstdlib>  // std::getenv
#include <ctime>
#include <iostream>
#include <string>

namespace lab {
namespace {

#ifdef NDEBUG
constexpr bool kShowLocationByDefault = false;
#else
constexpr bool kShowLocationByDefault = true;
#endif

constexpr std::string_view kGreen = "\033[32m";
constexpr std::string_view kBold = "\033[1m";
constexpr std::string_view kReset = "\033[0m";

bool stdoutSupportsColor() {
  // https://no-color.org: an existing NO_COLOR variable disables colors.
  if (std::getenv("NO_COLOR") != nullptr) {
    return false;
  }
  return ::isatty(STDOUT_FILENO) == 1;
}

std::string_view fileName(std::string_view path) {
  const auto slash = path.find_last_of("/\\");
  return slash == std::string_view::npos ? path : path.substr(slash + 1);
}

std::string currentTime() {
  const std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm local{};
  localtime_r(&now, &local);  // thread-safe variant of std::localtime

  char buffer[16]{};
  std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &local);
  return buffer;
}

}  // namespace

Logger& Logger::instance() {
  static Logger logger;
  return logger;
}

Logger::Logger()
    : show_location_{kShowLocationByDefault},
      use_color_{stdoutSupportsColor()} {}

void Logger::log(std::string_view message, const std::source_location& loc) {
  write(message, loc, /*with_function=*/true);
}

void Logger::logFunction(const std::source_location& loc) {
  // The function name is the message, so it is not repeated in the prefix.
  write(loc.function_name(), loc, /*with_function=*/false);
}

void Logger::section(std::string_view title) {
  constexpr std::size_t kWidth = 72;
  std::string line = "--- ";
  line.append(title);
  line.push_back(' ');
  if (line.size() < kWidth) {
    line.append(kWidth - line.size(), '-');
  }

  const std::lock_guard<std::mutex> lock(mutex_);
  std::cout << '\n';
  if (use_color_) {
    std::cout << kBold << line << kReset << '\n';
  } else {
    std::cout << line << '\n';
  }
}

void Logger::write(std::string_view message, const std::source_location& loc,
                   bool with_function) {
  if (!message.empty() && message.back() == '\n') {
    message.remove_suffix(1);
  }

  const std::lock_guard<std::mutex> lock(mutex_);
  if (show_location_) {
    std::cout << '[' << currentTime() << "][" << fileName(loc.file_name())
              << ':' << loc.line() << ']';
    if (with_function) {
      std::cout << '[' << loc.function_name() << ']';
    }
    std::cout << ' ';
  }
  if (use_color_) {
    std::cout << kGreen << message << kReset << '\n';
  } else {
    std::cout << message << '\n';
  }
}

}  // namespace lab
