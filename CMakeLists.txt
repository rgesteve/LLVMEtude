cmake_minimum_required(VERSION 3.8)
project(llvmetude)

find_package(LLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

#add_executable(llvmetude main.cpp)
