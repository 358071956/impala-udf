cmake_minimum_required(VERSION 2.6)

project (impalaudf)

aux_source_directory(. DIR_SRCS)
add_subdirectory(udf)

# where to put generated libraries
set(LIBRARY_OUTPUT_PATH "/root/Desktop/impala-udf/release")
# where to put generated binaries
set(EXECUTABLE_OUTPUT_PATH "/root/Desktop/impala-udf/release")

set(HIDE_SYMBOLS "-fvisibility=hidden -fvisibility-inlines-hidden")
set(HIDE_SYMBOLS_ARGS "${HIDE_SYMBOLS}")
separate_arguments(HIDE_SYMBOLS_ARGS)

#set(IR_COMPILE_FLAGS "-emit-llvm" "-O3" "-std=c++14" "-c" "-I../" ${HIDE_SYMBOLS_ARGS})
set(CLANG_BASE_FLAGS --gcc-toolchain=${GCC_ROOT})
set(CLANG_INCLUDE_FLAGS ${CLANG_BASE_FLAGS})
#set(IR_COMPILE_FLAGS "-emit-llvm" "-O3" "-std=gnu++14" "-c" "-I../" ${HIDE_SYMBOLS_ARGS})
set(IR_COMPILE_FLAGS "-emit-llvm" "-O3" "-std=c++11" "-c" "-I../" ${HIDE_SYMBOLS_ARGS})
set(IR_COMPILE_FLAGS ${IR_COMPILE_FLAGS} ${CLANG_BASE_FLAGS})

function(COMPILE_TO_IR SRC_FILE)
  get_filename_component(BASE_NAME ${SRC_FILE} NAME_WE)
  set(OUTPUT_FILE "${LIBRARY_OUTPUT_PATH}/${BASE_NAME}.ll")
  add_custom_command(
    OUTPUT ${OUTPUT_FILE}
    COMMAND clang++ ${IR_COMPILE_FLAGS} ${CLANG_INCLUDE_FLAGS} ${SRC_FILE} -o ${OUTPUT_FILE}
    DEPENDS ${SRC_FILE})
  add_custom_target(${BASE_NAME}-ir ALL DEPENDS ${OUTPUT_FILE})
endfunction(COMPILE_TO_IR)

# Build the UDA/UDFs into a shared library.
add_library(udfmy SHARED udf-my.cc)
set_target_properties(udfmy PROPERTIES COMPILE_FLAGS "${HIDE_SYMBOLS}")
add_dependencies(udfmy gen-deps)
add_library(udamy SHARED uda-my.cc)
set_target_properties(udamy PROPERTIES COMPILE_FLAGS "${HIDE_SYMBOLS}")
add_dependencies(udamy gen-deps)

# Custom targest to cross compile UDA/UDF to ir
COMPILE_TO_IR(udf-my.cc )
add_dependencies(udf-my-ir gen-deps)
COMPILE_TO_IR(uda-my.cc )
add_dependencies(uda-my-ir gen-deps)

