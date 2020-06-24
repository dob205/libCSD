cmake_minimum_required(VERSION 3.14)

project(test_libcsd)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../libcds/includes
    ${GTEST_INCLUDE_DIRS}
)

link_directories(
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../libcds/lib
)


add_executable(pfc_test pfc_test.cpp)
target_link_libraries(pfc_test  CSD cds  ${GTEST_BOTH_LIBRARIES} pthread)
