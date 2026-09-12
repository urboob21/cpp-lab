// Entry point of the cpp-lab application.
//
// Every example registers itself with LAB_EXAMPLE before main() starts, so
// main() only has to parse the command line and hand the registry to the menu
// or to the runner.

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "lab/CommandLine.h"
#include "lab/Logger.h"
#include "lab/Menu.h"
#include "lab/Registry.h"
#include "lab/Runner.h"
#include "lab/version.h"

namespace {

std::string cppStandard() {
  switch (__cplusplus) {
    case 202302L:
      return "C++23";
    case 202002L:
      return "C++20";
    case 201703L:
      return "C++17";
    case 201402L:
      return "C++14";
    case 201103L:
      return "C++11";
    default:
      return "__cplusplus = " + std::to_string(__cplusplus);
  }
}

void printVersion() {
  std::cout << lab::version::kName << " v" << lab::version::kVersion << '\n'
            << lab::version::kDescription << '\n'
            << "standard : " << cppStandard() << '\n'
            << "compiler : " << lab::version::kCompiler << '\n'
            << "build    : " << lab::version::kBuildType << " ("
            << lab::version::kBuildTime << ")\n";
}

void listExamples(const lab::Registry& registry, std::string_view filter) {
  const std::vector<const lab::Example*> matches = registry.match(filter);

  const auto longest =
      std::max_element(matches.begin(), matches.end(),
                       [](const lab::Example* a, const lab::Example* b) {
                         return a->id.size() < b->id.size();
                       });
  const std::size_t width =
      longest == matches.end() ? 0U : (*longest)->id.size();

  for (const lab::Example* example : matches) {
    std::cout << std::left << std::setw(static_cast<int>(width)) << example->id
              << "  " << example->description
              << (example->isInteractive() ? "  [interactive]" : "") << '\n';
  }
  std::cout << std::right << matches.size() << " example(s)\n";
}

void listIds(const lab::Registry& registry, std::string_view filter) {
  for (const lab::Example* example : registry.match(filter)) {
    if (!example->isInteractive()) {
      std::cout << example->id << '\n';
    }
  }
}

int runOne(const lab::Registry& registry, const std::string& query) {
  const lab::Example* example = registry.find(query);
  if (example == nullptr) {
    const std::vector<const lab::Example*> matches = registry.match(query);
    if (matches.empty()) {
      std::cerr << "error: no example matches '" << query << "' (see --list)\n";
      return 1;
    }
    if (matches.size() > 1) {
      std::cerr << "error: '" << query << "' matches " << matches.size()
                << " examples, be more specific:\n";
      for (const lab::Example* match : matches) {
        std::cerr << "  " << match->id << '\n';
      }
      return 1;
    }
    example = matches.front();
  }
  return lab::runExample(*example, std::cout) ? 0 : 1;
}

}  // namespace

int main(int argc, char* argv[]) {
  const std::vector<std::string_view> args(argv + 1, argv + argc);
  const std::string_view program = argc > 0 ? argv[0] : "cpp_lab_project";

  const lab::ParseResult parsed = lab::parseCommandLine(args);
  if (!parsed.value) {
    std::cerr << "error: " << parsed.error << "\n\n" << lab::usage(program);
    return 2;
  }
  const lab::CommandLine& cli = *parsed.value;

  if (cli.plain) {
    lab::Logger::instance().setShowLocation(false);
    lab::Logger::instance().setUseColor(false);
  }

  const lab::Registry& registry = lab::Registry::instance();
  if (!registry.errors().empty()) {
    for (const std::string& error : registry.errors()) {
      std::cerr << "registry error: " << error << '\n';
    }
    return 1;
  }

  switch (cli.command) {
    case lab::Command::kHelp:
      std::cout << lab::usage(program);
      return 0;
    case lab::Command::kVersion:
      printVersion();
      return 0;
    case lab::Command::kList:
      listExamples(registry, cli.argument);
      return 0;
    case lab::Command::kListIds:
      listIds(registry, cli.argument);
      return 0;
    case lab::Command::kRun:
      return runOne(registry, cli.argument);
    case lab::Command::kRunAll:
      return lab::runAll(registry, cli.argument, std::cout) == 0 ? 0 : 1;
    case lab::Command::kMenu:
      break;
  }

  printVersion();
  std::cout << "mode     : " << lab::toString(cli.mode) << '\n'
            << "examples : " << registry.examples().size()
            << "  (run with --help for command-line usage)\n";
  lab::runMenu(registry, std::cin, std::cout);
  std::cout << "Bye!\n";
  return 0;
}
