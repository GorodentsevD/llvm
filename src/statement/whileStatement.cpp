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
=================
 WHILE STATEMENT
=================
*/
void WhileStatement::codegen() {
    llvm::Function *function = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock *loop_block = llvm::BasicBlock::Create(context, "loop", function);
    builder.CreateBr(loop_block);

    builder.SetInsertPoint(loop_block);
    llvm::Value *stop_value = _cond->codegen();
    if (stop_value == nullptr)
        return;
    else if (stop_value->getType() != llvm::Type::getInt1Ty(context)) {
        yyerror("The condition of the if expression must be boolean");
    }

    llvm::BasicBlock *loop1_block = llvm::BasicBlock::Create(context, "loop1", function);
    llvm::BasicBlock *after_loop_block = llvm::BasicBlock::Create(context, "afterloop", function);
    builder.CreateCondBr(stop_value, loop1_block, after_loop_block);

    builder.SetInsertPoint(loop1_block);
    for (auto &i: *_then_stat)
        i->codegen();

    builder.CreateBr(loop_block);
    builder.SetInsertPoint(after_loop_block);
}
void WhileStatement::print(int level) const {
    std::cout << std::endl << prefix(level) << "#while structure:" << std::endl;
    if (_then_stat) {
        for (const auto &statement : *_then_stat) {
            statement->print(level + 2);
        }
    }
}

