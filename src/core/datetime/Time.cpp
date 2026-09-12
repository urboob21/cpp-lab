// -----------------------------------------------------------------------------
// Date and time (see README.md in this folder for the strftime format codes)
//
//   <ctime>   (from C)  std::time_t timestamps, std::tm broken-down time,
//                       localtime / gmtime / mktime, strftime formatting
//   <chrono>  (C++11)   type-safe clocks and durations
//                       (C++20) calendar types such as year_month_day
//
// Pitfall: std::localtime and std::gmtime return a pointer to shared static
// storage and are not thread-safe; POSIX localtime_r / gmtime_r are.
//
// Reference: https://en.cppreference.com/w/cpp/chrono/c
// -----------------------------------------------------------------------------

#include <chrono>
#include <ctime>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

std::string format(const std::tm& time, const char* pattern) {
  char buffer[64]{};
  const std::size_t length =
      std::strftime(buffer, sizeof(buffer), pattern, &time);
  return std::string(buffer, length);
}

void cTime() {
  LOG_SECTION("<ctime>");
  const std::time_t now = std::time(nullptr);  // seconds since 1970-01-01 UTC
  LOG_S("std::time(nullptr) = " << now);

  std::tm local{};
  localtime_r(&now, &local);  // thread-safe version of std::localtime
  std::tm utc{};
  gmtime_r(&now, &utc);

  LOG_S("year " << local.tm_year + 1900 << ", month " << local.tm_mon + 1
                << ", day " << local.tm_mday
                << "  (tm_year counts from 1900, tm_mon from 0)");
  LOG_S("local : " << format(local, "%Y-%m-%d %H:%M:%S"));
  LOG_S("UTC   : " << format(utc, "%Y-%m-%d %H:%M:%S"));
  LOG_S("%B %e, %Y     -> " << format(local, "%B %e, %Y"));
  LOG_S("%I:%M:%S %p   -> " << format(local, "%I:%M:%S %p"));
  LOG_S("%a %b %e %H:%M -> " << format(local, "%a %b %e %H:%M"));

  std::tm tomorrow = local;
  tomorrow.tm_mday += 1;  // may overflow the month...
  const std::time_t tomorrow_time =
      std::mktime(&tomorrow);  // ...mktime normalizes it
  LOG_S("tomorrow: " << format(tomorrow, "%Y-%m-%d") << " ("
                     << std::difftime(tomorrow_time, now) << " seconds later)");
}

void chronoCalendar() {
  LOG_SECTION("<chrono> calendar (C++20)");
  const auto now = std::chrono::system_clock::now();
  const auto today = std::chrono::floor<std::chrono::days>(now);
  const std::chrono::year_month_day date{today};

  LOG_S("year_month_day: " << static_cast<int>(date.year()) << '-'
                           << static_cast<unsigned>(date.month()) << '-'
                           << static_cast<unsigned>(date.day()) << " (UTC)");

  using namespace std::chrono;  // for the calendar literals below
  const year_month_day new_year = 2026y / January / 1d;
  const auto days_since = (today - sys_days{new_year}).count();
  LOG_S("days since 2026-01-01: " << days_since);
  LOG_S("2024 is a leap year: " << std::boolalpha << year{2024}.is_leap());

  const std::time_t as_time_t =
      system_clock::to_time_t(now);  // chrono -> C API
  LOG_S("system_clock::to_time_t(now) = " << as_time_t);
}

}  // namespace

LAB_EXAMPLE("Time",
            "<ctime> timestamps and strftime, <chrono> calendar dates") {
  cTime();
  chronoCalendar();
}
