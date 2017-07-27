#include "../ForceAllErrors.h"

#include <gtest/gtest.h>
#include <llvm/Object/Binary.h>
#include <llvm/Support/Error.h>

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
