cmake_minimum_required(VERSION 3.2)

cmake_policy(SET CMP0022 NEW) # enable post-2.8 behavior for INTERFACE_LINK_LIBRARIES
cmake_policy(SET CMP0048 NEW) # enable project versioning by CMake
cmake_policy(SET CMP0053 NEW) # simplify variable reference and escape sequence evaluation

find_package(GTest REQUIRED)
enable_testing()

add_executable(TestForceAllErrors
    TestDriver.cpp

    TestForceAllErrors.h
    TestDumpExample.h
    TestLLVMObject.h

    # unmodified LLVM source files for test
    ${LLVM_BUILD_MAIN_SRC_DIR}/include/llvm/Object/Archive.h
    ${LLVM_BUILD_MAIN_SRC_DIR}/include/llvm/Object/Binary.h
    ${LLVM_BUILD_MAIN_SRC_DIR}/include/llvm/Object/MachOUniversal.h
    ${LLVM_BUILD_MAIN_SRC_DIR}/include/llvm/Object/ObjectFile.h
    ${LLVM_BUILD_MAIN_SRC_DIR}/include/llvm/Support/GlobPattern.h
    ${LLVM_BUILD_MAIN_SRC_DIR}/include/llvm/Support/MemoryBuffer.h

    ${LLVM_BUILD_MAIN_SRC_DIR}/lib/Object/Archive.cpp
    ${LLVM_BUILD_MAIN_SRC_DIR}/lib/Object/Binary.cpp
    ${LLVM_BUILD_MAIN_SRC_DIR}/lib/Object/MachOUniversal.cpp
    ${LLVM_BUILD_MAIN_SRC_DIR}/lib/Object/ObjectFile.cpp
    ${LLVM_BUILD_MAIN_SRC_DIR}/lib/Support/GlobPattern.cpp
    ${LLVM_BUILD_MAIN_SRC_DIR}/lib/Support/MemoryBuffer.cpp

    #${LLVM_BUILD_MAIN_SRC_DIR}/include/llvm/Object/MachO.h
    #${LLVM_BUILD_MAIN_SRC_DIR}/lib/Object/MachOObjectFile.cpp

    # use these for demo errors on release_40
    ../llvm/Object/MachO.h
    ../llvm/Object/MachOObjectFile.cpp
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
