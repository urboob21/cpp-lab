#include <gtest/gtest.h>

#include <sstream>
#include <stdexcept>
#include <string>

#include "lab/Menu.h"
#include "lab/Registry.h"
#include "lab/Runner.h"

namespace {

int g_first_runs = 0;
int g_second_runs = 0;
int g_interactive_runs = 0;

void first() {
  ++g_first_runs;
}
void second() {
  ++g_second_runs;
}
void interactive() {
  ++g_interactive_runs;
}
void throws() {
  throw std::runtime_error("boom");
}

class MenuRunnerTest : public testing::Test {
 protected:
  void SetUp() override {
    g_first_runs = 0;
    g_second_runs = 0;
    g_interactive_runs = 0;
    registry.add("/x/src/topic/basics/A.cpp", "First", "first example", first);
    registry.add("/x/src/topic/advanced/B.cpp", "Second", "second example", second);
    registry.add("/x/src/topic/advanced/C.cpp", "Server", "needs a client", interactive,
                 lab::kInteractive);
  }

  lab::Registry registry;
  std::ostringstream out;
};

TEST_F(MenuRunnerTest, RunExampleReportsSuccess) {
  EXPECT_TRUE(lab::runExample(registry.examples().front(), out));
  EXPECT_NE(out.str().find("finished"), std::string::npos);
}

TEST_F(MenuRunnerTest, RunExampleCatchesExceptions) {
  lab::Registry failing;
  failing.add("/x/src/topic/Bad.cpp", "Bad", "throws", throws);
  EXPECT_FALSE(lab::runExample(failing.examples().front(), out));
  EXPECT_NE(out.str().find("boom"), std::string::npos);
  EXPECT_NE(out.str().find("FAILED"), std::string::npos);
}

TEST_F(MenuRunnerTest, RunAllSkipsInteractiveExamples) {
  EXPECT_EQ(lab::runAll(registry, "", out), 0);
  EXPECT_EQ(g_first_runs, 1);
  EXPECT_EQ(g_second_runs, 1);
  EXPECT_EQ(g_interactive_runs, 0);
  EXPECT_NE(out.str().find("skipped 1 interactive"), std::string::npos);
}

TEST_F(MenuRunnerTest, RunAllHonorsTheFilter) {
  EXPECT_EQ(lab::runAll(registry, "basics", out), 0);
  EXPECT_EQ(g_first_runs, 1);
  EXPECT_EQ(g_second_runs, 0);
}

TEST_F(MenuRunnerTest, MenuShowsFoldersAndQuits) {
  std::istringstream in("q\n");
  lab::runMenu(registry, in, out);
  const std::string text = out.str();
  EXPECT_NE(text.find("topic/"), std::string::npos);
  EXPECT_NE(text.find("3 examples"), std::string::npos);
}

TEST_F(MenuRunnerTest, MenuNavigatesFoldersAndRunsAnExample) {
  // root -> 1 (topic/) -> 2 (basics/, folders are sorted: advanced, basics)
  //      -> 1 (First) -> Enter to continue -> quit
  std::istringstream in("1\n2\n1\n\nq\n");
  lab::runMenu(registry, in, out);
  EXPECT_EQ(g_first_runs, 1);
  EXPECT_EQ(g_second_runs, 0);
}

TEST_F(MenuRunnerTest, MenuSearchRunsTheChosenMatch) {
  std::istringstream in("second\n1\n\nq\n");
  lab::runMenu(registry, in, out);
  EXPECT_EQ(g_second_runs, 1);
}

TEST_F(MenuRunnerTest, MenuEndsWhenInputEnds) {
  std::istringstream in("1\n");  // no quit command: EOF must not loop forever
  lab::runMenu(registry, in, out);
  SUCCEED();
}

TEST_F(MenuRunnerTest, BackFromTheTopFolderQuits) {
  std::istringstream in("0\n");
  lab::runMenu(registry, in, out);
  EXPECT_EQ(g_first_runs + g_second_runs, 0);
}

}  // namespace
