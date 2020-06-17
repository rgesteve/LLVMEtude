#include <iostream>
#include <cstdlib>

#include "llvm/Support/CommandLine.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/IR/DerivedTypes.h"

#include "llvm/IR/Verifier.h"

#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"

using namespace llvm;

static cl::opt<std::string> fileName(cl::Positional, cl::desc("Bitcode file"), cl::Required);

void read_file();

int main(int argc, char** argv)
{
    cl::ParseCommandLineOptions(argc, argv, "LLVMEtude: trying out stuff on the LLVM ecosystem\n");
    LLVMContext context;
    Module* mod = new Module("hello", context);
    IRBuilder<> builder(context);

#if LLVM_VERSION_MAJOR == 7
    std::cout << "Using LLVM version 7!" << std::endl;
#else
    std::cout << "A version of LLVM different than 7" << std::endl;
#endif

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
    verifyFunction(*putsFunc);

    mod->dump();
    std::cout << "--------------------------------" << std::endl;
    return EXIT_SUCCESS;
}

void read_file()
{

}