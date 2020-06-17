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

// static cl::opt<std::string> fileName(cl::Positional, cl::desc("Bitcode file"), cl::Required);
static cl::opt<std::string> fileName("i", cl::desc("Specify name of bitcode (.bc) file to parse"), cl::value_desc("filename"));

void generate_module(LLVMContext&);
void parse_module(const std::string&, LLVMContext&);

int main(int argc, char** argv)
{
    cl::ParseCommandLineOptions(argc, argv, "LLVMEtude: trying out stuff on the LLVM ecosystem\n");

#if LLVM_VERSION_MAJOR == 7
    std::cout << "Using LLVM version 7!" << std::endl;
#else
    std::cout << "A version of LLVM different than 7" << std::endl;
#endif

    LLVMContext context;
    if (!fileName.empty()) {
        llvm::errs() << "Should be used filename called [" << fileName << "] as input\n";
        parse_module(fileName, context);
    } else {
        llvm::errs() << "Generating a module on my own\n";
        generate_module(context);
    }

    return EXIT_SUCCESS;
}

void generate_module(LLVMContext& ctx)
{
    Module* mod = new Module("hello", ctx);
    IRBuilder<> builder(ctx);

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
}

void parse_module(const std::string& fname, LLVMContext& ctx)
{
    ErrorOr< std::unique_ptr<MemoryBuffer> > getFileResult = MemoryBuffer::getFile(fname);
    if (getFileResult.getError()) {
        llvm::errs() << "Can't read file [" << fname << "]\n";
        return;  // TODO: Should throw or encode an error or something
    }
    std::unique_ptr<MemoryBuffer>& buffer = getFileResult.get();
    Expected< std::unique_ptr<Module> > parseResult = parseBitcodeFile(*buffer.get(), ctx);
    if (auto err = parseResult.takeError()) {
        llvm::errs() << "Error while processing the bitcode file.\n";
        return;
    }
    std::unique_ptr<Module>& mod = parseResult.get();
    for (Module::const_iterator i = mod->getFunctionList().begin(),
                                e = mod->getFunctionList().end();  i != e; ++i) {
        if (!i->isDeclaration()) {
            llvm::outs() << i->getName() << " has " << i->size() << " basic block(s).\n";
        }
    }
}