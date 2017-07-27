#define DEBUG_TYPE "ForceAllErrors"

#include "../ForceAllErrors.h"

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/raw_ostream.h>

#include <gtest/gtest.h>

char *argv0; // for TestDumpExample

#include "TestForceAllErrors.h"
#include "TestDumpExample.h"
#include "TestLLVMObject.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(catch_exceptions) = false;

  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
  llvm::PrettyStackTraceProgram X(argc, argv);

  // Parse -debug and -debug-only options.
  llvm::cl::ParseCommandLineOptions(argc, argv, "ForceAllErrors TestDriver\n");

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
