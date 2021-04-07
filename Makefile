PROJECT = dynarmic
MAJVER = 3.0
MINVER = 0
LIBS = ${PROJECT}
PROGS = 

include make/conf
include make/cpplib
#include make/libpng.mk
#include make/zlib.mk

#Boost::boost
#Boost::serialization
#ELIBFLAGS += -lboost_serialization
CFLAGS = -DARCHITECTURE_x86_64=1 -DDYNARMIC_ENABLE_CPU_FEATURE_DETECTION=1 -DXBYAK_NO_OP_NAMES
CFLAGS += -Iexternals -I. -O2 -Wall -Wextra -std=c++17 -o

DATA =
DOCS =
INFOS =
INCLUDES =

## FMT
#SRC = externals/fmt/format.cc externals/fmt/os.cc
#  common/assert.cpp common/llvm_disassemble.cpp frontend/A32/disassembler_arm.cpp frontend/A32/disassembler_thumb.cpp frontend/A32/location_descriptor.cpp x64/perf_map.cpp
SRC += common/aes.cpp common/crc32.cpp common/sm4.cpp common/fused.cpp common/FPCompare.cpp common/FPConvert.cpp common/FPMulAdd.cpp common/FPRecipEstimate.cpp common/FPRecipExponent.cpp common/FPRecipStepFused.cpp common/FPRoundInt.cpp common/FPRSqrtEstimate.cpp common/FPRSqrtStepFused.cpp common/FPToFixed.cpp common/process_exception.cpp common/process_nan.cpp common/unpacked.cpp common/math_util.cpp common/memory_pool.cpp common/u128.cpp

SRC += frontend/basic_block.cpp frontend/opcodes.cpp
# frontend/A64/types.cpp frontend/location_descriptor.cpp
SRC += frontend/A32/types.cpp frontend/ir_emitter.cpp frontend/microinstruction.cpp frontend/type.cpp frontend/value.cpp ir_opt/constant_propagation_pass.cpp ir_opt/dead_code_elimination_pass.cpp ir_opt/identity_removal_pass.cpp ir_opt/verification_pass.cpp frontend/A32/ir_emitter.cpp frontend/A32/barrier.cpp frontend/A32/branch.cpp frontend/A32/coprocessor.cpp frontend/A32/crc32.cpp frontend/A32/data_processing.cpp frontend/A32/divide.cpp frontend/A32/exception_generating.cpp frontend/A32/extension.cpp frontend/A32/hint.cpp frontend/A32/load_store.cpp frontend/A32/misc.cpp frontend/A32/multiply.cpp frontend/A32/packing.cpp frontend/A32/parallel.cpp frontend/A32/reversal.cpp frontend/A32/saturated.cpp frontend/A32/status_register_access.cpp frontend/A32/synchronization.cpp frontend/A32/thumb16.cpp frontend/A32/thumb32.cpp frontend/A32/vfp.cpp frontend/A32/translate.cpp frontend/A32/translate_arm.cpp frontend/A32/translate_thumb.cpp

SRC += ir_opt/a32_constant_memory_reads_pass.cpp ir_opt/a32_get_set_elimination_pass.cpp x64/abi.cpp x64/block_of_code.cpp x64/block_range_information.cpp x64/callback.cpp x64/constant_pool.cpp x64/emit_x64.cpp x64/emit_x64_aes.cpp x64/emit_x64_crc32.cpp x64/emit_x64_data_processing.cpp x64/emit_x64_floating_point.cpp x64/emit_x64_packed.cpp x64/emit_x64_saturation.cpp x64/emit_x64_sm4.cpp x64/emit_x64_vector.cpp x64/emit_x64_vector_floating_point.cpp x64/hostloc.cpp x64/reg_alloc.cpp x64/a32_emit_x64.cpp x64/a32_interface.cpp x64/a32_jitstate.cpp x64/exception_handler_posix.cpp
LIBSRC = ${SRC}

PROGSRC =

include make/exconf.noasm
include make/build

dist-clean: clean

#${PROGS}:

include make/pack
include make/rules
include make/thedep
