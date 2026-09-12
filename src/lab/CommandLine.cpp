#include "lab/CommandLine.h"

#include <algorithm>
#include <cctype>

namespace lab {
namespace {

std::string toLower(std::string_view text) {
  std::string lower(text);
  std::transform(
      lower.begin(), lower.end(), lower.begin(),
      [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return lower;
}

ParseResult failure(std::string message) {
  return ParseResult{std::nullopt, std::move(message)};
}

}  // namespace

std::string_view toString(AppMode mode) {
  switch (mode) {
    case AppMode::kDev:
      return "dev";
    case AppMode::kUat:
      return "uat";
    case AppMode::kProd:
      return "prod";
  }
  return "unknown";
}

std::optional<AppMode> parseAppMode(std::string_view text) {
  const std::string mode = toLower(text);
  if (mode == "dev") {
    return AppMode::kDev;
  }
  if (mode == "uat") {
    return AppMode::kUat;
  }
  if (mode == "prod") {
    return AppMode::kProd;
  }
  return std::nullopt;
}

ParseResult parseCommandLine(const std::vector<std::string_view>& args) {
  CommandLine cli;
  bool command_given = false;

  for (std::size_t i = 0; i < args.size(); ++i) {
    std::string_view option = args[i];

    // Support both "--run id" and "--run=id".
    std::optional<std::string_view> inline_value;
    if (option.starts_with("--")) {
      if (const auto equals = option.find('=');
          equals != std::string_view::npos) {
        inline_value = option.substr(equals + 1);
        option = option.substr(0, equals);
      }
    }

    // Returns the value that follows the option, if there is one.
    auto take_value = [&]() -> std::optional<std::string_view> {
      if (inline_value) {
        return inline_value;
      }
      if (i + 1 < args.size() && !args[i + 1].starts_with('-')) {
        return args[++i];
      }
      return std::nullopt;
    };

    auto set_command = [&](Command command) {
      if (command_given) {
        return false;
      }
      cli.command = command;
      command_given = true;
      return true;
    };
    const std::string one_command_error =
        "only one of --list, --list-ids, --run, --run-all, --help and "
        "--version can be used at a time";

    if (option == "-h" || option == "--help") {
      if (!set_command(Command::kHelp)) {
        return failure(one_command_error);
      }
    } else if (option == "-v" || option == "--version") {
      if (!set_command(Command::kVersion)) {
        return failure(one_command_error);
      }
    } else if (option == "-l" || option == "--list" || option == "--list-ids" ||
               option == "-a" || option == "--run-all") {
      Command command = Command::kRunAll;
      if (option == "-l" || option == "--list") {
        command = Command::kList;
      } else if (option == "--list-ids") {
        command = Command::kListIds;
      }
      if (!set_command(command)) {
        return failure(one_command_error);
      }
      if (auto value = take_value()) {
        cli.argument = std::string(*value);
      }
    } else if (option == "-r" || option == "--run") {
      if (!set_command(Command::kRun)) {
        return failure(one_command_error);
      }
      auto value = take_value();
      if (!value || value->empty()) {
        return failure("--run needs an example id or filter");
      }
      cli.argument = std::string(*value);
    } else if (option == "--plain") {
      cli.plain = true;
    } else if (option == "--mode" || option == "-mode") {
      auto value = take_value();
      if (!value) {
        return failure("--mode needs a value: dev, uat or prod");
      }
      auto mode = parseAppMode(*value);
      if (!mode) {
        return failure("invalid mode '" + std::string(*value) +
                       "', expected dev, uat or prod");
      }
      cli.mode = *mode;
    } else {
      return failure("unknown option '" + std::string(args[i]) + "'");
    }
  }

  return ParseResult{cli, {}};
}

std::string usage(std::string_view program) {
  std::string text = "Usage: ";
  text.append(program);
  text.append(R"( [options]

Without options the interactive menu starts.

Options:
  -l, --list [filter]      list examples (id and description)
  -r, --run <id|filter>    run one example: an exact id, or a filter that
                           matches exactly one example
  -a, --run-all [filter]   run every non-interactive example
      --list-ids [filter]  print ids of non-interactive examples (for scripts)
      --plain              plain output: no log prefixes, no colors
      --mode <mode>        application mode: dev (default), uat or prod
  -v, --version            print version information
  -h, --help               print this help

Filters are case-insensitive substrings of the id or the description.

Examples:
  )");
  text.append(program);
  text.append(" --list smart_pointer\n  ");
  text.append(program);
  text.append(" --run core/smart_pointer/Weak\n  ");
  text.append(program);
  text.append(" --run-all dp/behavioral\n");
  return text;
}

}  // namespace lab
