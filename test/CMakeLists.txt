cmake_minimum_required(VERSION 3.14)

project(test_libcsd)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(GTest QUIET)
if(GTest_FOUND)
add_executable(pfc_test pfc_test.cpp)
target_link_libraries(pfc_test  CSD cds  ${GTEST_BOTH_LIBRARIES} pthread)

add_executable(hrpdac_blocks_test hrpdac_blocks_test.cpp)
target_link_libraries(hrpdac_blocks_test  CSD cds  ${GTEST_BOTH_LIBRARIES} pthread)


add_executable(parallel_test parallel_test.cpp)
target_link_libraries(parallel_test  CSD cds  ${GTEST_BOTH_LIBRARIES} pthread)

endif()