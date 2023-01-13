cmake_minimum_required (VERSION 3.12)

project ("base64_test")

include_directories(doctest)

enable_testing()

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
add_test(NAME base64_test COMMAND ./base64_test)

# NOTE: Don't use space inside a generator expression here, because the function prematurely breaks the expression into
# multiple lines. https://cmake.org/pipermail/cmake/2018-April/067422.html
set(SJTEST_GCC_FLAGS -Wall -Wextra -Wpedantic)
set(SJTEST_CLANG_FLAGS -Weverything -Wno-global-constructors -Wno-weak-vtables)
target_compile_options(
  ${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W3> $<$<CXX_COMPILER_ID:GNU>:${SJTEST_GCC_FLAGS}>
                       $<$<CXX_COMPILER_ID:Clang>:${SJTEST_CLANG_FLAGS}>)

message(STATUS "CMAKE_CURRENT_BINARY_DIR=" ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)


