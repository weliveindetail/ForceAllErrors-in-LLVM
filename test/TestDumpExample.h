#include "../ForceAllErrors.h"

#include <gtest/gtest.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/GlobPattern.h>

bool dumpExample(const char *fileName) {
  using namespace llvm;
  StringRef fileList = "...";

  if (Expected<GlobPattern> P = GlobPattern::create(fileName))
    return P->match(fileList); // success case, frequently taken
  else
    consumeError(P.takeError()); // error case, rarely taken, low test coverage

  return false;
}

TEST(ForceAllErrors, dumpExample)
{
  auto test_dumpExample = []() { dumpExample(argv0); };
  int C = CountMutationPoints(test_dumpExample);

  //ForceAllErrors(C, test_dumpExample); // uncomment to debug

  EXPECT_EXIT(ForceAllErrors(C, test_dumpExample),
              ::testing::ExitedWithCode(0), "");
}
