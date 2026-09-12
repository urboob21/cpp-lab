#include "lab/CommandLine.h"

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

namespace {

lab::ParseResult parse(std::vector<std::string_view> args) {
  return lab::parseCommandLine(args);
}

TEST(CommandLineTest, NoArgumentsStartsTheMenu) {
  const lab::ParseResult result = parse({});
  ASSERT_TRUE(result.value.has_value()) << result.error;
  EXPECT_EQ(result.value->command, lab::Command::kMenu);
  EXPECT_FALSE(result.value->plain);
  EXPECT_EQ(result.value->mode, lab::AppMode::kDev);
}

TEST(CommandLineTest, ListAcceptsAnOptionalFilter) {
  auto without_filter = parse({"--list"});
  ASSERT_TRUE(without_filter.value.has_value());
  EXPECT_EQ(without_filter.value->command, lab::Command::kList);
  EXPECT_TRUE(without_filter.value->argument.empty());

  auto with_filter = parse({"-l", "pointer"});
  ASSERT_TRUE(with_filter.value.has_value());
  EXPECT_EQ(with_filter.value->argument, "pointer");
}

TEST(CommandLineTest, RunRequiresAnId) {
  auto result = parse({"--run", "core/smart_pointer/Weak"});
  ASSERT_TRUE(result.value.has_value());
  EXPECT_EQ(result.value->command, lab::Command::kRun);
  EXPECT_EQ(result.value->argument, "core/smart_pointer/Weak");

  EXPECT_FALSE(parse({"--run"}).value.has_value());
  EXPECT_FALSE(
      parse({"--run", "--plain"}).value.has_value());  // an option is not an id
}

TEST(CommandLineTest, SupportsEqualsSyntax) {
  auto result = parse({"--run=dp/Proxy", "--mode=prod"});
  ASSERT_TRUE(result.value.has_value()) << result.error;
  EXPECT_EQ(result.value->argument, "dp/Proxy");
  EXPECT_EQ(result.value->mode, lab::AppMode::kProd);
}

TEST(CommandLineTest, OptionsCanBeCombinedWithACommand) {
  auto result = parse({"--plain", "--run-all", "dp", "--mode", "UAT"});
  ASSERT_TRUE(result.value.has_value()) << result.error;
  EXPECT_EQ(result.value->command, lab::Command::kRunAll);
  EXPECT_EQ(result.value->argument, "dp");
  EXPECT_TRUE(result.value->plain);
  EXPECT_EQ(result.value->mode, lab::AppMode::kUat);
}

TEST(CommandLineTest, RejectsTwoCommands) {
  const lab::ParseResult result = parse({"--list", "--run-all"});
  EXPECT_FALSE(result.value.has_value());
  EXPECT_NE(result.error.find("only one of"), std::string::npos);
}

TEST(CommandLineTest, RejectsUnknownOptionsAndInvalidModes) {
  EXPECT_FALSE(parse({"--frobnicate"}).value.has_value());
  EXPECT_FALSE(parse({"--mode", "staging"}).value.has_value());
  EXPECT_FALSE(parse({"--mode"}).value.has_value());
}

TEST(CommandLineTest, LegacySingleDashModeStillWorks) {
  auto result = parse({"-mode", "Prod"});
  ASSERT_TRUE(result.value.has_value());
  EXPECT_EQ(result.value->mode, lab::AppMode::kProd);
}

TEST(AppModeTest, RoundTripsThroughStrings) {
  for (const lab::AppMode mode :
       {lab::AppMode::kDev, lab::AppMode::kUat, lab::AppMode::kProd}) {
    const auto parsed = lab::parseAppMode(lab::toString(mode));
    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(*parsed, mode);
  }
}

TEST(UsageTest, MentionsEveryCommand) {
  const std::string text = lab::usage("cpp_lab_project");
  for (const char* option : {"--list", "--run", "--run-all", "--list-ids",
                             "--plain", "--mode", "--version", "--help"}) {
    EXPECT_NE(text.find(option), std::string::npos) << option;
  }
}

}  // namespace
