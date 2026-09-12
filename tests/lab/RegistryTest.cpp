#include "lab/Registry.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

namespace {

void noop() {}

std::vector<std::string> ids(const std::vector<const lab::Example*>& examples) {
  std::vector<std::string> result;
  for (const lab::Example* example : examples) {
    result.push_back(example->id);
  }
  return result;
}

TEST(RelativeSourcePathTest, StripsTheSourceRoot) {
  EXPECT_EQ(lab::detail::relativeSourcePath("/repo/src/core/Weak.cpp", "/repo"),
            "src/core/Weak.cpp");
  EXPECT_EQ(lab::detail::relativeSourcePath("/repo/src/core/Weak.cpp", "/repo/"),
            "src/core/Weak.cpp");
}

TEST(RelativeSourcePathTest, FallsBackToTheLastSrcDirectory) {
  EXPECT_EQ(lab::detail::relativeSourcePath("/elsewhere/src/dp/Proxy.cpp", "/repo"),
            "src/dp/Proxy.cpp");
  EXPECT_EQ(lab::detail::relativeSourcePath("src/dp/Proxy.cpp", ""), "src/dp/Proxy.cpp");
}

TEST(RelativeSourcePathTest, NormalizesBackslashes) {
  EXPECT_EQ(lab::detail::relativeSourcePath(R"(C:\repo\src\core\Weak.cpp)", R"(C:\repo)"),
            "src/core/Weak.cpp");
}

TEST(GroupFromSourcePathTest, UsesTheDirectoryBelowSrc) {
  EXPECT_EQ(lab::detail::groupFromSourcePath("src/core/smart_pointer/Weak.cpp"),
            "core/smart_pointer");
  EXPECT_EQ(lab::detail::groupFromSourcePath("src/dp/Proxy.cpp"), "dp");
  EXPECT_EQ(lab::detail::groupFromSourcePath("tests/lab/RegistryTest.cpp"), "tests/lab");
}

TEST(GroupFromSourcePathTest, FilesWithoutADirectoryGoToMisc) {
  EXPECT_EQ(lab::detail::groupFromSourcePath("src/main.cpp"), "misc");
  EXPECT_EQ(lab::detail::groupFromSourcePath("main.cpp"), "misc");
}

class RegistryTest : public testing::Test {
 protected:
  lab::Registry registry;  // a fresh registry per test, not the global one
};

TEST_F(RegistryTest, BuildsIdGroupAndSourceFromTheFilePath) {
  ASSERT_TRUE(registry.add("/x/src/core/smart_pointer/Weak.cpp", "Weak", "weak_ptr", noop));

  ASSERT_EQ(registry.examples().size(), 1U);
  const lab::Example& example = registry.examples().front();
  EXPECT_EQ(example.id, "core/smart_pointer/Weak");
  EXPECT_EQ(example.group, "core/smart_pointer");
  EXPECT_EQ(example.name, "Weak");
  EXPECT_EQ(example.description, "weak_ptr");
  EXPECT_EQ(example.source, "src/core/smart_pointer/Weak.cpp");
  EXPECT_FALSE(example.isInteractive());
  EXPECT_TRUE(registry.errors().empty());
}

TEST_F(RegistryTest, KeepsExamplesSortedCaseInsensitively) {
  registry.add("/x/src/b/File.cpp", "zeta", "", noop);
  registry.add("/x/src/a/File.cpp", "Beta", "", noop);
  registry.add("/x/src/a/File.cpp", "alpha", "", noop);

  std::vector<std::string> order;
  for (const lab::Example& example : registry.examples()) {
    order.push_back(example.id);
  }
  EXPECT_EQ(order, (std::vector<std::string>{"a/alpha", "a/Beta", "b/zeta"}));
}

TEST_F(RegistryTest, RejectsDuplicateIdsIgnoringCase) {
  EXPECT_TRUE(registry.add("/x/src/core/One.cpp", "Same", "", noop));
  EXPECT_FALSE(registry.add("/x/src/core/Two.cpp", "same", "", noop));

  EXPECT_EQ(registry.examples().size(), 1U);
  ASSERT_EQ(registry.errors().size(), 1U);
  EXPECT_NE(registry.errors().front().find("duplicate"), std::string::npos);
}

TEST_F(RegistryTest, RejectsInvalidNamesAndMissingFunctions) {
  EXPECT_FALSE(registry.add("/x/src/core/A.cpp", "", "", noop));
  EXPECT_FALSE(registry.add("/x/src/core/A.cpp", "has space", "", noop));
  EXPECT_FALSE(registry.add("/x/src/core/A.cpp", "has/slash", "", noop));
  EXPECT_FALSE(registry.add("/x/src/core/A.cpp", "NoFunction", "", nullptr));

  EXPECT_TRUE(registry.examples().empty());
  EXPECT_EQ(registry.errors().size(), 4U);
}

TEST_F(RegistryTest, StoresFlags) {
  registry.add("/x/src/socket/Server.cpp", "Server", "", noop, lab::kInteractive);
  EXPECT_TRUE(registry.examples().front().isInteractive());
}

TEST_F(RegistryTest, FindPrefersExactMatches) {
  registry.add("/x/src/core/A.cpp", "Weak", "", noop);
  registry.add("/x/src/other/A.cpp", "weak", "", noop);

  ASSERT_NE(registry.find("core/Weak"), nullptr);
  EXPECT_EQ(registry.find("core/Weak")->id, "core/Weak");
  ASSERT_NE(registry.find("CORE/WEAK"), nullptr);  // case-insensitive fallback
  EXPECT_EQ(registry.find("CORE/WEAK")->id, "core/Weak");
  EXPECT_EQ(registry.find("core/Strong"), nullptr);
}

TEST_F(RegistryTest, MatchSearchesIdsAndDescriptions) {
  registry.add("/x/src/core/smart_pointer/A.cpp", "Weak", "observe without owning", noop);
  registry.add("/x/src/core/smart_pointer/B.cpp", "Unique", "exclusive ownership", noop);
  registry.add("/x/src/dp/C.cpp", "Proxy", "a stand-in object", noop);

  EXPECT_EQ(registry.match("").size(), 3U);
  EXPECT_EQ(ids(registry.match("SMART_POINTER")),
            (std::vector<std::string>{"core/smart_pointer/Unique", "core/smart_pointer/Weak"}));
  EXPECT_EQ(ids(registry.match("owning")), (std::vector<std::string>{"core/smart_pointer/Weak"}));
  EXPECT_TRUE(registry.match("nothing matches this").empty());
}

}  // namespace
