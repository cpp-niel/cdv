cmake_minimum_required(VERSION 3.15)

project(cdv CXX)
include(cmake/StandardProjectSettings.cmake)

find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)


add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(project_options INTERFACE -fcoroutines)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if (ENABLE_BUILD_WITH_TIME_TRACE)
    add_compile_definitions(project_options INTERFACE -ftime-trace)
  endif()
endif()
 
add_library(project_warnings INTERFACE)

include(cmake/Cache.cmake)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

include(cmake/Doxygen.cmake)
enable_doxygen()

include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_APPROVAL_TESTING "Enable Approval Test Builds" ON)
option(ENABLE_UNIT_TESTING "Enable Unit Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
  target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility>)
endif()

if(ENABLE_APPROVAL_TESTING)
  enable_testing()
  message(
          "Building Approval Tests."
  )
  add_subdirectory(tests/approval_tests)
endif()

if(ENABLE_UNIT_TESTING)
  enable_testing()
  message(
    "Building Unit Tests."
  )
  add_subdirectory(tests/unit_tests)
endif()

if(ENABLE_FUZZING)
  message(
    "Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html"
  )
  add_subdirectory(tests/fuzz_test)
endif()

add_subdirectory(src)


option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
if (ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  set_target_properties(intro PROPERTIES UNITY_BUILD ON)
endif()


