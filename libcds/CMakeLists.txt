cmake_minimum_required(VERSION 3.14)

project(cds)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


set(bitsequence_srcs 
src/bitsequence/BitSequence.cpp
src/bitsequence/BitSequence375.cpp
src/bitsequence/BitSequenceBuilder375.cpp
src/bitsequence/BitSequenceBuilderDArray.cpp
src/bitsequence/BitSequenceBuilderRG.cpp
src/bitsequence/BitSequenceBuilderRRR.cpp
src/bitsequence/BitSequenceBuilderSDArray.cpp
src/bitsequence/BitSequenceDArray.cpp
src/bitsequence/BitSequenceRG.cpp
src/bitsequence/BitSequenceRRR.cpp
src/bitsequence/BitSequenceSDArray.cpp
src/bitsequence/sdarraySadakane.cpp
src/bitsequence/TableOffsetRRR.cpp
)

set(coders_srcs
src/coders/huff.cpp
src/coders/HuffmanCoder.cpp
)

set(mapper_srcs
src/mapper/Mapper.cpp
src/mapper/MapperCont.cpp
src/mapper/MapperNone.cpp
)

set(permutation_srcs
src/permutation/perm.cpp
src/permutation/Permutation.cpp
src/permutation/PermutationBuilderMRRR.cpp
src/permutation/PermutationMRRR.cpp
)

set(sequence_srcs
src/sequence/BitmapsSequence.cpp
src/sequence/Sequence.cpp
src/sequence/SequenceAlphPart.cpp
src/sequence/SequenceBuilderAlphPart.cpp
src/sequence/SequenceBuilderGMR.cpp
src/sequence/SequenceGMRChunk.cpp
# src/sequence/SequenceBuilderStr.cpp
src/sequence/SequenceBuilderWaveletTree.cpp
src/sequence/SequenceBuilderWaveletTreeNoptrs.cpp
src/sequence/SequenceGMR.cpp
src/sequence/SequenceGMRChunk.cpp
src/sequence/WaveletTree.cpp
src/sequence/WaveletTreeNoptrs.cpp
src/sequence/wt_coder.cpp
src/sequence/wt_coder_binary.cpp
src/sequence/wt_coder_huff.cpp
src/sequence/wt_node.cpp
src/sequence/wt_node_internal.cpp
src/sequence/wt_node_leaf.cpp
)

set(suffixtree_srcs
src/suffixtree/factorization.cpp
src/suffixtree/factorization_var.cpp
src/suffixtree/LCP.cpp
src/suffixtree/LCP_DAC.cpp
src/suffixtree/LCP_DAC_VAR.cpp
src/suffixtree/LCP_FMN.cpp
src/suffixtree/LCP_naive.cpp
src/suffixtree/LCP_PhiSpare.cpp
src/suffixtree/LCP_PT.cpp
src/suffixtree/LCP_Sad.cpp
src/suffixtree/NPR.cpp
src/suffixtree/NPR_CN.cpp
src/suffixtree/NPR_FMN.cpp
src/suffixtree/NSV.cpp
src/suffixtree/PSV.cpp
src/suffixtree/RMQ_succinct.cpp
src/suffixtree/RMQ_succinct_lcp.cpp
src/suffixtree/SuffixTree.cpp
src/suffixtree/SuffixTreeY.cpp
)

set(textindex_srcs
src/textindex/comparray4.cpp
src/textindex/mmap.cpp
src/textindex/qsufsort.cpp
src/textindex/suftest3.test.cpp
src/textindex/TextIndex.cpp
src/textindex/TextIndexCSA.cpp
)

set(utils_srcs
src/utils/Array.cpp
src/utils/BitString.cpp
src/utils/cppUtils.cpp
)


include_directories(includes)

add_library(csd STATIC 
${bitsequence_srcs}
${coders_srcs}
${mapper_srcs}
${permutation_srcs}
${sequence_srcs}
${suffixtree_srcs}
${textindex_srcs}
${utils_srcs}
)