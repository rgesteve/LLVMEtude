cmake_minimum_required(VERSION 3.8)
find_package(LLVM REQUIRED CONFIG)

project(llvmetude)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(LLVM_LINK_COMPONENTS
  Core # maybe over version 10, this is LLVMCore
  BitWriter
  Support
  )

add_executable(llvmetude main.cpp)

#add_llvm_executable(llvmetude main.cpp)

# Find the libraries that correspond to the LLVM components that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(llvmetude ${llvm_libs})

# ----------------
#set_target_properties(Hello PROPERTIES COMPILE_FLAGS "")
#get_target_property(MAIN_CFLAGS Hello COMPILE_FLAGS)
#message("-- Target Compiler flags are: ${MAIN_CFLAGS}")
