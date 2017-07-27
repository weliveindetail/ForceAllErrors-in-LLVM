#include "../ForceAllErrors.h"

#include <gtest/gtest.h>
#include <llvm/Support/Error.h>

TEST(ForceAllErrors, uncheckedFails)
{
#if !GTEST_OS_WINDOWS
  using namespace llvm;

  auto test_uncheckedSuccess = []() { Expected<int> I(3); };
  EXPECT_EXIT(test_uncheckedSuccess(),
              ::testing::KilledBySignal(SIGABRT), "");

  auto test_uncheckedError = []() { Expected<int> I(mockError()); };
  EXPECT_EXIT(test_uncheckedError(),
              ::testing::KilledBySignal(SIGABRT), "");
#endif
}
