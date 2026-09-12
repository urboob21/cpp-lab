#pragma once

#include <atomic>
#include <mutex>
#include <source_location>
#include <sstream>  // IWYU pragma: keep (used by LOG_S)
#include <string_view>

namespace lab {

/// Thread-safe line logger used by the examples.
///
/// Debug builds prefix every line with the time, file:line and the calling
/// function, so you can see where each line of output comes from. Release
/// builds (NDEBUG) and `--plain` print the bare message.
class Logger {
 public:
  static Logger& instance();

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  /// Prints one line. A single trailing '\n' in `message` is dropped.
  void log(std::string_view message,
           const std::source_location& loc = std::source_location::current());

  /// Prints the full signature of the calling function. Handy to see which
  /// overload, constructor or operator the compiler picked.
  void logFunction(
      const std::source_location& loc = std::source_location::current());

  /// Prints a visual separator: "--- title -----------".
  void section(std::string_view title);

  void setShowLocation(bool show) { show_location_ = show; }
  void setUseColor(bool use) { use_color_ = use; }
  bool showLocation() const { return show_location_; }

 private:
  Logger();
  void write(std::string_view message, const std::source_location& loc,
             bool with_function);

  std::mutex mutex_;
  std::atomic<bool> show_location_;
  std::atomic<bool> use_color_;
};

}  // namespace lab

/// LOG("text") - log a string (anything convertible to std::string_view).
#define LOG(message) ::lab::Logger::instance().log(message)

/// LOG_S("x = " << x) - log anything that can be streamed into std::ostream.
#define LOG_S(...)                                        \
  do {                                                    \
    std::ostringstream lab_log_stream_;                   \
    lab_log_stream_ << __VA_ARGS__;                       \
    ::lab::Logger::instance().log(lab_log_stream_.str()); \
  } while (false)

/// LOG_FUNC() - log the signature of the current function.
#define LOG_FUNC() ::lab::Logger::instance().logFunction()

/// LOG_SECTION("Title") - print a separator to structure the output.
#define LOG_SECTION(title) ::lab::Logger::instance().section(title)
