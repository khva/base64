cmake_minimum_required (VERSION 3.15 FATAL_ERROR)

project ("base64")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/impl)

enable_testing()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(tests)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)


