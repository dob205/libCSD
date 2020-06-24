cmake_minimum_required(VERSION 3.14)

project(CSD)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_subdirectory(libcds)
include_directories(libcds/includes)

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


add_library(CSD
${base_srcs}
${fmindex_srcs}
${hash_srcs}
${huffman_srcs}
${hutucker_srcs}
${repair_srcs}
)

add_dependencies(CSD cds)
