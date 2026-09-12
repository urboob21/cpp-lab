#include "lab/Runner.h"

#include <chrono>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace lab {
namespace {

constexpr std::size_t kRuleWidth = 72;

}  // namespace

bool runExample(const Example& example, std::ostream& out) {
  const std::string heavy_rule(kRuleWidth, '=');
  const std::string light_rule(kRuleWidth, '-');

  out << '\n'
      << heavy_rule << '\n'
      << "> " << example.id
      << (example.isInteractive() ? "  [interactive]" : "") << '\n'
      << "  " << example.description << '\n'
      << "  source: " << example.source << '\n'
      << heavy_rule << '\n'
      << std::flush;

  // Examples may change the formatting state of std::cout (std::hex,
  // std::boolalpha, ...). Save it so the next example starts clean.
  std::ios saved_format(nullptr);
  saved_format.copyfmt(std::cout);

  const auto start = std::chrono::steady_clock::now();
  bool succeeded = true;
  try {
    example.run();
  } catch (const std::exception& e) {
    succeeded = false;
    std::cout.flush();
    out << "\n[error] uncaught exception: " << e.what() << '\n';
  } catch (...) {
    succeeded = false;
    std::cout.flush();
    out << "\n[error] uncaught exception of unknown type\n";
  }
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::steady_clock::now() - start);

  std::cout.copyfmt(saved_format);
  std::cout.flush();
  std::cerr.flush();

  out << light_rule << '\n'
      << (succeeded ? "finished" : "FAILED") << " in " << elapsed.count()
      << " ms\n"
      << std::flush;
  return succeeded;
}

int runAll(const Registry& registry, std::string_view filter,
           std::ostream& out) {
  std::vector<std::string> failed;
  std::size_t ran = 0;
  std::size_t skipped = 0;

  for (const Example* example : registry.match(filter)) {
    if (example->isInteractive()) {
      ++skipped;
      continue;
    }
    ++ran;
    if (!runExample(*example, out)) {
      failed.push_back(example->id);
    }
  }

  out << '\n'
      << std::string(kRuleWidth, '=') << '\n'
      << "Ran " << ran << " example(s), skipped " << skipped
      << " interactive, " << failed.size() << " failed.\n";
  for (const std::string& id : failed) {
    out << "  FAILED: " << id << '\n';
  }
  out << std::flush;
  return static_cast<int>(failed.size());
}

}  // namespace lab
