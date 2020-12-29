cmake_minimum_required(VERSION 3.14)

project(CSD)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_subdirectory(libcds)
include_directories(libcds/includes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(base_srcs 
StringDictionary.cpp
StringDictionaryFMINDEX.cpp
StringDictionaryHASHHF.cpp
StringDictionaryHASHRPDAC.cpp
StringDictionaryHASHRPF.cpp
StringDictionaryHASHUFFDAC.cpp
StringDictionaryHHTFC.cpp
StringDictionaryHTFC.cpp
StringDictionaryPFC.cpp
StringDictionaryRPDAC.cpp
StringDictionaryRPFC.cpp
StringDictionaryRPHTFC.cpp
StringDictionaryXBW.cpp
StringDictionaryHASHRPDACBlocks.cpp
)

set(fmindex_srcs
FMIndex/SSA.cpp
FMIndex/SuffixArray.cpp
)

set(hash_srcs
Hash/HashBBdh.cpp
Hash/HashBdh.cpp
Hash/Hash.cpp
Hash/HashDAC.cpp
Hash/Hashdh.cpp
)

set(huffman_srcs
Huffman/huff.cpp
Huffman/Huffman.cpp
)

set(hutucker_srcs
HuTucker/HuTucker.cpp
)

set(repair_srcs
RePair/Coder/arrayg.cpp
RePair/Coder/basics.cpp
RePair/Coder/CRePair.cpp
RePair/Coder/dictionary.cpp
RePair/Coder/hash.cpp
RePair/Coder/heap.cpp
RePair/Coder/IRePair.cpp
RePair/Coder/records.cpp
RePair/RePair.cpp
)

set(utils_srcs
utils/DAC_BVLS.cpp
utils/DAC_VLS.cpp
utils/LogSequence.cpp
utils/VByte.cpp

utils/Coder/BinaryNode.cpp
utils/Coder/Coder.cpp
utils/Coder/DecodingTableBuilder.cpp
utils/Coder/DecodingTable.cpp
utils/Coder/DecodingTree.cpp
utils/Coder/StatCoder.cpp
)

set(xbw_srcs
XBW/TrieNode.cpp
XBW/XBW.cpp
)

add_library(CSD
${base_srcs}
${fmindex_srcs}
${hash_srcs}
${huffman_srcs}
${hutucker_srcs}
${repair_srcs}
${utils_srcs}
${xbw_srcs}
)

add_dependencies(CSD cds)


find_package(GTest QUIET)
if(GTest_FOUND)
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(pfc_test test/pfc_test.cpp)
target_link_libraries(pfc_test  CSD cds  ${GTEST_BOTH_LIBRARIES} pthread)

add_executable(hrpdac_blocks_test  test/hrpdac_blocks_test.cpp)
target_link_libraries(hrpdac_blocks_test  CSD cds  ${GTEST_BOTH_LIBRARIES} pthread)


add_executable(parallel_test  test/parallel_test.cpp)
target_link_libraries(parallel_test  CSD cds  ${GTEST_BOTH_LIBRARIES} pthread)

add_test(NAME pfc_test COMMAND ./pfc_test)
add_test(NAME hrpdac_blocks_test COMMAND ./hrpdac_blocks_test)
add_test(NAME parallel_test COMMAND ./parallel_test)

endif()