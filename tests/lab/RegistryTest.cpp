#include "lab/Registry.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

namespace {

void noop() {}

std::vector<std::string> ids(const std::vector<const lab::Example*>& examples) {
  std::vector<std::string> result;
  result.reserve(examples.size());
  for (const lab::Example* example : examples) {
    result.push_back(example->id);
  }
  return result;
}

TEST(RelativeSourcePathTest, StripsTheSourceRoot) {
  EXPECT_EQ(lab::detail::relativeSourcePath("/repo/src/core/Weak.cpp", "/repo"),
            "src/core/Weak.cpp");
  EXPECT_EQ(
      lab::detail::relativeSourcePath("/repo/src/core/Weak.cpp", "/repo/"),
      "src/core/Weak.cpp");
}

TEST(RelativeSourcePathTest, FallsBackToTheLastSrcDirectory) {
  EXPECT_EQ(
      lab::detail::relativeSourcePath("/elsewhere/src/dp/Proxy.cpp", "/repo"),
      "src/dp/Proxy.cpp");
  EXPECT_EQ(lab::detail::relativeSourcePath("src/dp/Proxy.cpp", ""),
            "src/dp/Proxy.cpp");
}

TEST(RelativeSourcePathTest, NormalizesBackslashes) {
  EXPECT_EQ(lab::detail::relativeSourcePath(R"(C:\repo\src\core\Weak.cpp)",
                                            R"(C:\repo)"),
            "src/core/Weak.cpp");
}

TEST(GroupFromSourcePathTest, UsesTheDirectoryBelowSrc) {
  EXPECT_EQ(lab::detail::groupFromSourcePath("src/core/smart_pointer/Weak.cpp"),
            "core/smart_pointer");
  EXPECT_EQ(lab::detail::groupFromSourcePath("src/dp/Proxy.cpp"), "dp");
  EXPECT_EQ(lab::detail::groupFromSourcePath("tests/lab/RegistryTest.cpp"),
            "tests/lab");
}

TEST(GroupFromSourcePathTest, FilesWithoutADirectoryGoToMisc) {
  EXPECT_EQ(lab::detail::groupFromSourcePath("src/main.cpp"), "misc");
  EXPECT_EQ(lab::detail::groupFromSourcePath("main.cpp"), "misc");
}

class RegistryTest : public testing::Test {
 protected:
  lab::Registry registry_;  // a fresh registry per test, not the global one
};

TEST_F(RegistryTest, BuildsIdGroupAndSourceFromTheFilePath) {
  ASSERT_TRUE(registry_.add("/x/src/core/smart_pointer/Weak.cpp", "Weak",
                            "weak_ptr", noop));

  ASSERT_EQ(registry_.examples().size(), 1U);
  const lab::Example& example = registry_.examples().front();
  EXPECT_EQ(example.id, "core/smart_pointer/Weak");
  EXPECT_EQ(example.group, "core/smart_pointer");
  EXPECT_EQ(example.name, "Weak");
  EXPECT_EQ(example.description, "weak_ptr");
  EXPECT_EQ(example.source, "src/core/smart_pointer/Weak.cpp");
  EXPECT_FALSE(example.isInteractive());
  EXPECT_TRUE(registry_.errors().empty());
}

TEST_F(RegistryTest, KeepsExamplesSortedCaseInsensitively) {
  registry_.add("/x/src/b/File.cpp", "zeta", "", noop);
  registry_.add("/x/src/a/File.cpp", "Beta", "", noop);
  registry_.add("/x/src/a/File.cpp", "alpha", "", noop);

  std::vector<std::string> order;
  for (const lab::Example& example : registry_.examples()) {
    order.push_back(example.id);
  }
  EXPECT_EQ(order, (std::vector<std::string>{"a/alpha", "a/Beta", "b/zeta"}));
}

TEST_F(RegistryTest, RejectsDuplicateIdsIgnoringCase) {
  EXPECT_TRUE(registry_.add("/x/src/core/One.cpp", "Same", "", noop));
  EXPECT_FALSE(registry_.add("/x/src/core/Two.cpp", "same", "", noop));

  EXPECT_EQ(registry_.examples().size(), 1U);
  ASSERT_EQ(registry_.errors().size(), 1U);
  EXPECT_NE(registry_.errors().front().find("duplicate"), std::string::npos);
}

TEST_F(RegistryTest, RejectsInvalidNamesAndMissingFunctions) {
  EXPECT_FALSE(registry_.add("/x/src/core/A.cpp", "", "", noop));
  EXPECT_FALSE(registry_.add("/x/src/core/A.cpp", "has space", "", noop));
  EXPECT_FALSE(registry_.add("/x/src/core/A.cpp", "has/slash", "", noop));
  EXPECT_FALSE(registry_.add("/x/src/core/A.cpp", "NoFunction", "", nullptr));

  EXPECT_TRUE(registry_.examples().empty());
  EXPECT_EQ(registry_.errors().size(), 4U);
}

TEST_F(RegistryTest, StoresFlags) {
  registry_.add("/x/src/socket/Server.cpp", "Server", "", noop,
                lab::kInteractive);
  EXPECT_TRUE(registry_.examples().front().isInteractive());
  EXPECT_FALSE(registry_.examples().front().isDraft());
  EXPECT_EQ(registry_.examples().front().labels(), "  [interactive]");
}

TEST_F(RegistryTest, LabelsDraftAndInteractiveExamples) {
  registry_.add("/x/src/core/A.cpp", "Plain", "", noop);
  registry_.add("/x/src/core/B.cpp", "Draft", "", noop, lab::kDraft);
  registry_.add("/x/src/core/C.cpp", "Both", "", noop,
                lab::kDraft | lab::kInteractive);

  EXPECT_EQ(registry_.find("core/Plain")->labels(), "");
  EXPECT_TRUE(registry_.find("core/Draft")->isDraft());
  EXPECT_EQ(registry_.find("core/Draft")->labels(), "  [draft]");
  EXPECT_EQ(registry_.find("core/Both")->labels(), "  [draft]  [interactive]");
}

TEST_F(RegistryTest, FindPrefersExactMatches) {
  registry_.add("/x/src/core/A.cpp", "Weak", "", noop);
  registry_.add("/x/src/other/A.cpp", "weak", "", noop);

  ASSERT_NE(registry_.find("core/Weak"), nullptr);
  EXPECT_EQ(registry_.find("core/Weak")->id, "core/Weak");
  ASSERT_NE(registry_.find("CORE/WEAK"), nullptr);  // case-insensitive fallback
  EXPECT_EQ(registry_.find("CORE/WEAK")->id, "core/Weak");
  EXPECT_EQ(registry_.find("core/Strong"), nullptr);
}

TEST_F(RegistryTest, MatchSearchesIdsAndDescriptions) {
  registry_.add("/x/src/core/smart_pointer/A.cpp", "Weak",
                "observe without owning", noop);
  registry_.add("/x/src/core/smart_pointer/B.cpp", "Unique",
                "exclusive ownership", noop);
  registry_.add("/x/src/dp/C.cpp", "Proxy", "a stand-in object", noop);

  EXPECT_EQ(registry_.match("").size(), 3U);
  EXPECT_EQ(ids(registry_.match("SMART_POINTER")),
            (std::vector<std::string>{"core/smart_pointer/Unique",
                                      "core/smart_pointer/Weak"}));
  EXPECT_EQ(ids(registry_.match("owning")),
            (std::vector<std::string>{"core/smart_pointer/Weak"}));
  EXPECT_TRUE(registry_.match("nothing matches this").empty());
}

}  // namespace
