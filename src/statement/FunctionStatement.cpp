#include "statement.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <iostream>
#include "alloc/alloc.hpp"

extern llvm::LLVMContext context;
extern std::map<std::string, llvm::AllocaInst *> named_values;
extern llvm::IRBuilder<> builder;
extern llvm::Module *module;
llvm::Value *Str;

extern void yyerror(std::string msg);


/*
====================
 FUNCTION STATEMENT
====================
*/
void FunctionAST::codegen() {
    llvm::Function *function = module->getFunction(m_prototype->getId());

    function = function == nullptr ? m_prototype->codegen(): function;
    if (function == nullptr) {
        yyerror("ERROR: fail of function codegen");
        return;
    }

    if (!function->empty()) {
        yyerror("ERROR: Cannot redefine function: " + m_prototype->getId());
    }

    llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(basic_block);

    named_values.clear();
    for (auto &arg : function->args()) {
        llvm::AllocaInst *alloca = create_entry_block_alloca(function, arg.getName(), arg.getType());

        builder.CreateStore(&arg, alloca);

        named_values[arg.getName()] = alloca;
    }

    for (Statement *statement : m_block) {
        statement->codegen();
    }

    llvm::verifyFunction(*function);
}

FunctionAST::~FunctionAST() {
    delete m_prototype;
}

void FunctionAST::print(int level) const {
    std::cout << prefix(level) << "function: " << std::endl;
    m_prototype->print(level + 1);
    std::cout << std::endl << prefix(level) << "body:" << std::endl;
    for (const auto &statement : m_block) {
        statement->print(level + 1);
    }
}
