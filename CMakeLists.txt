cmake_minimum_required(VERSION 3.2)

cmake_policy(SET CMP0022 NEW) # enable >2.8 behavior for INTERFACE_LINK_LIBRARIES
cmake_policy(SET CMP0048 NEW) # enable project versioning by CMake
cmake_policy(SET CMP0053 NEW) # simplify variable reference and escape sequence evaluation

project(ForceAllErrors VERSION 1.0 LANGUAGES CXX)

message("")
message("Dependencies")

if(WIN32)
    set(OS_NAME "Win")
    set(DISABLE_RTTI_FLAG /GR-)
elseif(APPLE)
    set(OS_NAME "OSX")
    set(DISABLE_RTTI_FLAG -fno-rtti)
elseif(UNIX)
    set(OS_NAME "Linux")
    set(DISABLE_RTTI_FLAG -fno-rtti)
else()
    message(FATAL_ERROR "Operating system not supported")
endif()
message(STATUS "System: ${OS_NAME}")

add_executable(ForceAllErrors
    ForceAllErrors.cpp
)

set_target_properties(ForceAllErrors PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add LLVM libraries, set LLVM_* variables
find_package(LLVM 4.0 REQUIRED PATHS ${LLVM_DIR})
message(STATUS "LLVM_DIR: ${LLVM_DIR}")
message(STATUS "LLVM_PACKAGE_VERSION: ${LLVM_PACKAGE_VERSION}")

if(NOT LLVM_ENABLE_RTTI)
    target_compile_options(ForceAllErrors PRIVATE ${DISABLE_RTTI_FLAG})
endif()

message(STATUS "LLVM Source Directory: ${LLVM_BUILD_MAIN_SRC_DIR}")
message(STATUS "LLVM Build Directory: ${LLVM_BUILD_BINARY_DIR}")

target_include_directories(ForceAllErrors PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LLVM_INCLUDE_DIRS}
)

# LLVM definitions
target_compile_definitions(ForceAllErrors PRIVATE
    ${LLVM_DEFINITIONS}
)

# ForceAllErrors dependencies
llvm_map_components_to_libnames(LLVM_LIBS
    core
    orcjit
    x86asmparser
    x86codegen
)

target_link_libraries(ForceAllErrors PRIVATE
    ${LLVM_LIBS}
)

message("")
message("ForceAllErrors")

get_target_property(ALL_ID ForceAllErrors INCLUDE_DIRECTORIES)
message(STATUS "Include directories: ${ALL_ID}")

get_target_property(ALL_LL ForceAllErrors LINK_LIBRARIES)
message(STATUS "Link libraries: ${ALL_LL}")

get_target_property(ALL_CO ForceAllErrors COMPILE_OPTIONS)
message(STATUS "Compile options: ${ALL_CO}")

get_target_property(ALL_CD ForceAllErrors COMPILE_DEFINITIONS)
message(STATUS "Compile definitions: ${ALL_CD}")

message(STATUS "Other flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Other flags Debug: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "Other flags Release: ${CMAKE_CXX_FLAGS_RELEASE}")