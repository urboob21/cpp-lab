#include "lab/Menu.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "lab/Runner.h"

namespace lab {
namespace {

constexpr std::size_t kRuleWidth = 72;
constexpr int kNameWidth = 26;

/// One folder of the menu. The root folder has an empty name and path.
struct MenuNode {
  std::string name;  ///< last path segment, e.g. "smart_pointer"
  std::string path;  ///< full group, e.g. "core/smart_pointer"
  std::vector<MenuNode> children;
  std::vector<const Example*> examples;
  std::size_t total{0};  ///< examples in this folder and all sub-folders
};

MenuNode& childNamed(MenuNode& parent, const std::string& name) {
  const auto it = std::find_if(
      parent.children.begin(), parent.children.end(),
      [&name](const MenuNode& child) { return child.name == name; });
  if (it != parent.children.end()) {
    return *it;
  }
  MenuNode child;
  child.name = name;
  child.path = parent.path.empty() ? name : parent.path + "/" + name;
  parent.children.push_back(std::move(child));
  return parent.children.back();
}

void sortChildren(MenuNode& node) {
  std::sort(
      node.children.begin(), node.children.end(),
      [](const MenuNode& a, const MenuNode& b) { return a.name < b.name; });
  for (MenuNode& child : node.children) {
    sortChildren(child);
  }
}

/// Turns the flat list of "group/name" ids into a folder tree.
MenuNode buildTree(const Registry& registry) {
  MenuNode root;
  for (const Example& example : registry.examples()) {
    MenuNode* node = &root;
    ++node->total;

    std::string_view rest = example.group;
    while (!rest.empty()) {
      const auto slash = rest.find('/');
      const std::string segment(rest.substr(0, slash));
      rest = slash == std::string_view::npos ? std::string_view{}
                                             : rest.substr(slash + 1);
      node = &childNamed(*node, segment);
      ++node->total;
    }
    node->examples.push_back(&example);
  }
  sortChildren(root);
  return root;
}

std::string trim(const std::string& text) {
  const auto first = text.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) {
    return {};
  }
  const auto last = text.find_last_not_of(" \t\r\n");
  return text.substr(first, last - first + 1);
}

std::optional<std::size_t> parseIndex(const std::string& text) {
  constexpr std::size_t kMaxDigits = 6;
  if (text.empty() || text.size() > kMaxDigits ||
      !std::all_of(text.begin(), text.end(),
                   [](unsigned char c) { return std::isdigit(c) != 0; })) {
    return std::nullopt;
  }
  return static_cast<std::size_t>(std::stoul(text));
}

std::string countLabel(std::size_t count) {
  return std::to_string(count) + (count == 1 ? " example" : " examples");
}

void printNode(const MenuNode& node, bool is_root, std::ostream& out) {
  const std::string rule(kRuleWidth, '-');
  std::string location = "cpp-lab";
  if (!node.path.empty()) {
    location += " / ";
    for (const char c : node.path) {
      location += c == '/' ? std::string(" / ") : std::string(1, c);
    }
  }

  out << '\n'
      << rule << '\n'
      << ' ' << location << "   (" << countLabel(node.total) << ")\n"
      << rule << '\n';

  std::size_t index = 1;
  for (const MenuNode& child : node.children) {
    out << std::setw(4) << index++ << ". " << std::left << std::setw(kNameWidth)
        << (child.name + "/") << std::right << ' ' << countLabel(child.total)
        << '\n';
  }
  for (const Example* example : node.examples) {
    out << std::setw(4) << index++ << ". " << std::left << std::setw(kNameWidth)
        << example->name << std::right << ' ' << example->description
        << (example->isInteractive() ? "  [interactive]" : "") << '\n';
  }
  out << rule << '\n'
      << " <number> open    0 " << (is_root ? "quit" : "back")
      << "    <text> search    ? help    q quit\n";
}

void printHelp(std::ostream& out) {
  out << "\nMenu help\n"
         "  <number>  open a folder or run an example\n"
         "  0, b      go back one folder (quit in the top folder)\n"
         "  <text>    search ids and descriptions, e.g. \"thread\" or "
         "\"dp/\"\n"
         "  q         quit\n\n"
         "Tip: run examples without the menu, e.g.\n"
         "  cpp_lab_project --list pointer\n"
         "  cpp_lab_project --run core/smart_pointer/Weak\n";
}

/// Prints the prompt and reads one trimmed line. False when input ended.
bool readLine(std::istream& in, std::ostream& out, std::string& line) {
  out << "> " << std::flush;
  if (!std::getline(in, line)) {
    return false;
  }
  line = trim(line);
  return true;
}

/// Runs an example and waits for Enter. False when input ended.
bool runAndWait(const Example& example, std::istream& in, std::ostream& out) {
  runExample(example, out);

  // An example that reads from std::cin may leave it in a failed state.
  if (in.fail() && !in.eof()) {
    in.clear();
  }
  if (!in) {
    return false;
  }
  out << "\nPress Enter to return to the menu..." << std::flush;
  std::string ignored;
  return static_cast<bool>(std::getline(in, ignored));
}

/// Shows the examples matching `query` and runs the chosen one.
/// False when input ended.
bool search(const Registry& registry, std::string_view query, std::istream& in,
            std::ostream& out) {
  const std::vector<const Example*> matches = registry.match(query);
  if (matches.empty()) {
    out << "No example matches \"" << query << "\".\n";
    return true;
  }

  out << "\nExamples matching \"" << query << "\":\n";
  for (std::size_t i = 0; i < matches.size(); ++i) {
    out << std::setw(4) << i + 1 << ". " << std::left << std::setw(40)
        << matches[i]->id << std::right << ' ' << matches[i]->description
        << '\n';
  }
  out << " <number> run    0 cancel\n";

  std::string line;
  if (!readLine(in, out, line)) {
    return false;
  }
  const auto index = parseIndex(line);
  if (!index || *index == 0 || *index > matches.size()) {
    return true;
  }
  return runAndWait(*matches[*index - 1], in, out);
}

}  // namespace

void runMenu(const Registry& registry, std::istream& in, std::ostream& out) {
  if (registry.examples().empty()) {
    out << "No examples registered.\n";
    return;
  }

  const MenuNode root = buildTree(registry);
  std::vector<const MenuNode*> path{&root};
  std::string line;

  while (true) {
    const MenuNode& node = *path.back();
    const bool is_root = path.size() == 1;
    printNode(node, is_root, out);

    if (!readLine(in, out, line)) {
      out << '\n';
      return;
    }
    if (line.empty()) {
      continue;
    }
    if (line == "q" || line == "quit" || line == "exit") {
      return;
    }
    if (line == "0" || line == "b" || line == "..") {
      if (is_root) {
        return;
      }
      path.pop_back();
      continue;
    }
    if (line == "?" || line == "h" || line == "help") {
      printHelp(out);
      continue;
    }

    if (const auto index = parseIndex(line)) {
      const std::size_t folders = node.children.size();
      if (*index <= folders) {
        path.push_back(&node.children[*index - 1]);
      } else if (*index - folders <= node.examples.size()) {
        if (!runAndWait(*node.examples[*index - folders - 1], in, out)) {
          return;
        }
      } else {
        out << "Invalid choice: " << line << '\n';
      }
      continue;
    }

    const std::string_view query =
        line.front() == '/' ? std::string_view(line).substr(1) : line;
    if (!search(registry, query, in, out)) {
      return;
    }
  }
}

}  // namespace lab
