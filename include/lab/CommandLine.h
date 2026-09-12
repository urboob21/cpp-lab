#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace lab {

enum class Command {
  kMenu,     ///< interactive menu (default)
  kList,     ///< --list [filter]
  kListIds,  ///< --list-ids [filter]
  kRun,      ///< --run <id|filter>
  kRunAll,   ///< --run-all [filter]
  kHelp,     ///< --help
  kVersion,  ///< --version
};

enum class AppMode { kDev, kUat, kProd };

struct CommandLine {
  Command command{Command::kMenu};
  std::string argument;  ///< example id or filter, depending on the command
  bool plain{false};     ///< no log prefixes, no colors
  AppMode mode{AppMode::kDev};
};

struct ParseResult {
  std::optional<CommandLine> value;  ///< set on success
  std::string error;                 ///< set on failure
};

/// Parses the program arguments (without argv[0]).
ParseResult parseCommandLine(const std::vector<std::string_view>& args);

/// Help text for `--help`.
std::string usage(std::string_view program);

std::string_view toString(AppMode mode);

/// Accepts "dev", "uat" and "prod" (case-insensitive).
std::optional<AppMode> parseAppMode(std::string_view text);

}  // namespace lab
