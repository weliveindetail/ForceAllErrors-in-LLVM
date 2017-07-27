# ForceAllErrors

Hook into `llvm::Expected` and `llvm::ErrorOr` to turn success cases into error cases for testing. 

The gtest driver counts the number `N` of valid `llvm::Expected` and `llvm::ErrorOr` instances, that are created while 
running a piece of code (like the below `dumpExample()` function). It then re-executes the code `i = 1..N` times and 
turns the `i`th valid instance into an error instance. Thus each error path will be taken once and potential issues 
show up (like the below nullptr dereference).

## Dump Example

The following code is very unlikely to hit the nullptr deref, 
because `GlobPattern::create()` will rarely fail 
when given a file name as input:
```cpp
bool dumpExample(const char *fileName) {
  using namespace llvm;
  StringRef fileList = "...";

  if (Expected<GlobPattern> P = GlobPattern::create(fileName))
    return P->match(fileList); // success case, frequently taken
  else
    consumeError(P.takeError()); // error case, rarely taken, low test coverage

 int x = *(int*)0; // <-- runtime error, unlikely to show up in regular tests
 return false;
}

int main(int argc, char **argv) {
  return dumpExample(argv[0]) ? 0 : 1;
}
```

The test that uncovers this issue:
```cpp
#include "../ForceAllErrors.h"

#include <gtest/gtest.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/GlobPattern.h>

TEST(ForceAllErrors, dumpExample)
{
  auto test_dumpExample = []() { dumpExample(argv0); };
  int C = CountMutationPoints(test_dumpExample);

  //ForceAllErrors(C, test_dumpExample); // uncomment to debug

  EXPECT_EXIT(ForceAllErrors(C, test_dumpExample),
              ::testing::ExitedWithCode(0), "");
}
```

Checkout the [demo-dumpExample](https://github.com/weliveindetail/ForceAllErrors-in-LLVM/commits/demo-dumpExample) branch and see yourself!

## Real-world Example

You may think that the above example mostly indicates missing unit tests or insufficient test data. And that this can also be figured from test coverage reports easily. Right, that's why here comes the real-world example with real benefits.

The test runs `llvm::object::createBinary()` and detects 44 different control paths that will all be executed one after the other. These cases can hardly be covered by a unit test altogether, because they don't depend on the single input to `creatBinary()` but rather on the environment the test runs in. Simulating this in a unit test would be a real challenge.

```cpp
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

  EXPECT_EXIT(ForceAllErrors(C, test_createBinary),
              ::testing::ExitedWithCode(0), "");
}
```

Checkout the [demo-MachOObjectFile-release_40](https://github.com/weliveindetail/ForceAllErrors-in-LLVM/commits/demo-MachOObjectFile-release_40) branch. It has a copy of `MachOObjectFile.cpp` from its `release_40` original with [2 runtime errors inserted in error paths](https://github.com/weliveindetail/ForceAllErrors-in-LLVM/commit/ac96849de1ba5c9b88f1fceb87dbf6b3aa8fcd1b). You can watch it detect the issues!

## Build
```
$ cmake -G Xcode -DLLVM_DIR=/path/to/llvm40-build-xcode/lib/cmake/llvm ../ForceAllErrors
-- The CXX compiler identification is AppleClang 8.0.0.8000038
-- Check for working CXX compiler
-- Check for working CXX compiler -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- System: OSX

LLVM
-- LLVM_DIR: /path/to/llvm40-build-xcode/lib/cmake/llvm
-- LLVM_PACKAGE_VERSION: 4.0.1
-- LLVM Source Directory: /path/to/llvm
-- LLVM Build Directory: /path/to/llvm40-build-xcode

GoogleTest
-- Found GTest: /usr/local/lib/libgtest.a  
-- Looking for C++ include pthread.h
-- Looking for C++ include pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - found
-- Found Threads: TRUE  

ForceAllErrors
-- Include directories: /Develop/ForceAllErrors/ForceAllErrors;/path/to/llvm/include;/path/to/llvm40-build-xcode/include
-- Compile options: -fno-rtti
-- Compile definitions: __STDC_CONSTANT_MACROS;__STDC_FORMAT_MACROS;__STDC_LIMIT_MACROS
-- Other flags: 
-- Other flags Debug: -g
-- Other flags Release: -O3 -DNDEBUG
-- Configuring done
-- Generating done
-- Build files have been written to: /Develop/ForceAllErrors/build-xcode

$ cmake --build .
```

## Run

Use the command line argument `-debug` to dump the number of mutation points 
for each test to the test output. This is the expected output for 
[master](https://github.com/weliveindetail/ForceAllErrors-in-LLVM/commits/master) 
on Mac OSX 10.12:

```
$ cd test/Debug
$ ./TestForceAllErrors -debug
Args: ./TestForceAllErrors -debug 
[==========] Running 3 tests from 2 test cases.
[----------] Global test environment set-up.
[----------] 2 tests from ForceAllErrors
[ RUN      ] ForceAllErrors.uncheckedFails
[       OK ] ForceAllErrors.uncheckedFails (62 ms)
[ RUN      ] ForceAllErrors.dumpExample
             Detected 1 mutation points
[       OK ] ForceAllErrors.dumpExample (2 ms)
[----------] 2 tests from ForceAllErrors (64 ms total)

[----------] 1 test from LLVMObject
[ RUN      ] LLVMObject.createBinary
             Detected 44 mutation points
[       OK ] LLVMObject.createBinary (11 ms)
[----------] 1 test from LLVMObject (11 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 2 test cases ran. (76 ms total)
[  PASSED  ] 3 tests.
```
