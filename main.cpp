#include <iostream>
#include <cstdlib>

#include "llvm/IR/LLVMContext.h"

using namespace llvm;

int main()
{
    LLVMContext context;
    std::cout << "Hello, changed world (now with llvm)!" << std::endl;
    return EXIT_SUCCESS;
}