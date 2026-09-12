// -----------------------------------------------------------------------------
// Regular expressions (<regex>)
//
//   std::regex_match    does the WHOLE string match?     (validation)
//   std::regex_search   is there a match ANYWHERE?       (searching)
//   std::smatch         the match and its capture groups  (extraction)
//   std::sregex_iterator iterate over all matches
//   std::regex_replace  replace matches ($& whole match, $1 first group)
//
// Tips:
//   - Use raw string literals R"(...)" so backslashes need no escaping.
//   - Constructing a std::regex is expensive: build it once and reuse it.
//   - std::regex is known to be slow; fine for learning and small inputs.
//
// Reference: https://en.cppreference.com/w/cpp/regex
// -----------------------------------------------------------------------------

#include <iterator>
#include <regex>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

const std::string kText =
    "This is my string. I'll use regular expressions. Now I have two problems.";

void searching() {
  LOG_SECTION("regex_search: find a match anywhere");
  LOG_S("text: " << kText);
  const std::regex phrase(R"(regular expression)", std::regex::icase);
  LOG_S("contains 'REGULAR EXPRESSION' (ignoring case): "
        << std::boolalpha << std::regex_search(kText, phrase));
}

void validation() {
  LOG_SECTION("regex_match: validate the whole string");
  const std::regex phone(
      R"(^\+[0-9]{8,15}$)");  // '+' followed by 8 to 15 digits
  for (const std::string candidate :
       {"+84336821417", "0336821417", "+84 33 682"}) {
    LOG_S(candidate << " -> "
                    << (std::regex_match(candidate, phone) ? "valid"
                                                           : "invalid"));
  }
}

void captureGroups() {
  LOG_SECTION("Capture groups with std::smatch");
  const std::regex iso_date(R"((\d{4})-(\d{2})-(\d{2}))");
  const std::string log_line = "backup finished on 2026-09-11 at 23:00";

  std::smatch match;
  if (std::regex_search(log_line, match, iso_date)) {
    LOG_S("whole match: " << match[0] << " at position " << match.position(0));
    LOG_S("year = " << match[1] << ", month = " << match[2]
                    << ", day = " << match[3]);
  }
}

void iterating() {
  LOG_SECTION("Iterating over all matches");
  const std::regex word(R"(\w+)");
  const auto begin = std::sregex_iterator(kText.begin(), kText.end(), word);
  const auto end = std::sregex_iterator();
  LOG_S("found " << std::distance(begin, end) << " words");

  std::string long_words;
  for (auto it = begin; it != end; ++it) {
    if (it->str().size() > 5) {
      long_words += it->str() + " ";
    }
  }
  LOG_S("words longer than 5 characters: " << long_words);
}

void replacing() {
  LOG_SECTION("regex_replace");
  const std::regex long_word(R"(\w{6,})");
  LOG_S("[$&] around long words: " << std::regex_replace(kText, long_word,
                                                         "[$&]"));

  const std::regex date(R"((\d{4})-(\d{2})-(\d{2}))");
  LOG_S("2026-09-11 reordered with $3/$2/$1: " << std::regex_replace(
            std::string("2026-09-11"), date, "$3/$2/$1"));
}

}  // namespace

LAB_EXAMPLE(
    "Regex",
    "regex_match, regex_search, capture groups, iterators and regex_replace") {
  searching();
  validation();
  captureGroups();
  iterating();
  replacing();
}
