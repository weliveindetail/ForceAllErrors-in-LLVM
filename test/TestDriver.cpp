#define DEBUG_TYPE "ForceAllErrors"

#include "../ForceAllErrors.h"

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/raw_ostream.h>

#include <gtest/gtest.h>

#include <memory>
#include <string>

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

#include <llvm/Object/Binary.h>
const char *argv0;

TEST(LLVMObject, createBinary)
{
  using namespace llvm;
  using namespace llvm::object;

  auto test_createBinary = []() {
    Expected<OwningBinary<Binary>> bin = createBinary(argv0);
    if (!bin)
      consumeError(bin.takeError());
  };

  int C = CountMutationPoints(test_createBinary);

  // uncomment to debug
  //ForceAllErrors(C, test_createBinary);

  EXPECT_EXIT(ForceAllErrors(C, test_createBinary),
              ::testing::ExitedWithCode(0), "");
}

// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(catch_exceptions) = false;

  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
  llvm::PrettyStackTraceProgram X(argc, argv);

  // Parse -debug and -debug-only options.
  llvm::cl::ParseCommandLineOptions(argc, argv, "ForceAllErrors Driver\n");

  argv0 = argv[0];
  int exitCode = RUN_ALL_TESTS();

  // On windows don't close the command prompt immediately.
  #ifdef _WIN32
    fflush(stderr);
    fflush(stdout);
    getchar();
  #endif

  return exitCode;
}
