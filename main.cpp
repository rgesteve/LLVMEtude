#include <iostream>
#include <cstdlib>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/IR/DerivedTypes.h"

using namespace llvm;

int main()
{
    LLVMContext context;
    Module* mod = new Module("hello", context);
    IRBuilder<> builder(context);
    // std::cout << "Hello, changed world (now with llvm module)!" << std::endl;

    // Create a reference to external function `puts`
    std::vector<Type*> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    ArrayRef<Type*> argsRef(putsArgs);

    FunctionType* putsType = FunctionType::get(builder.getInt32Ty(), // result type
                                               argsRef, // parameter array
                                               false // this is not a vararg function
    );
    //FunctionCallee putsFunc = mod->getOrInsertFunction("puts", putsType); // this may not be available in llvm 7.0
    Function* putsFunc = Function::Create(putsType, Function::ExternalLinkage, "puts", mod);

    mod->dump();
    std::cout << "--------------------------------" << std::endl;
    return EXIT_SUCCESS;
}