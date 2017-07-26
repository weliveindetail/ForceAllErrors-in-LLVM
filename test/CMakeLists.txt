cmake_minimum_required(VERSION 3.2)

cmake_policy(SET CMP0022 NEW) # enable post-2.8 behavior for INTERFACE_LINK_LIBRARIES
cmake_policy(SET CMP0048 NEW) # enable project versioning by CMake
cmake_policy(SET CMP0053 NEW) # simplify variable reference and escape sequence evaluation

find_package(GTest REQUIRED)
enable_testing()

add_executable(TestForceAllErrors
    TestDriver.cpp

    # unmodified LLVM source files for test
    ../llvm/Object/Binary.h
    ../llvm/Support/MemoryBuffer.h

    ../llvm/Object/Binary.cpp
    ../llvm/Support/MemoryBuffer.cpp
)

set_target_properties(TestForceAllErrors PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(TestForceAllErrors PRIVATE 
    ForceAllErrors
    GTest::GTest
)
