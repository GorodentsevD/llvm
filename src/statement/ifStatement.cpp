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
==============
 IF STATEMENT
==============
*/
void IfStatement::codegen() {
    llvm::Value *cond_value = _cond->codegen();

    if (cond_value->getType() != llvm::Type::getInt1Ty(context)) {
        yyerror("The condition of the IF expression must be boolean");
    }

    llvm::Function *function = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock *then_block = llvm::BasicBlock::Create(context, "iftrue", function);
    llvm::BasicBlock *else_block = llvm::BasicBlock::Create(context, "iffalse");
    llvm::BasicBlock *merge_block = llvm::BasicBlock::Create(context, "ifcont");

    builder.CreateCondBr(cond_value, then_block, else_block);
    builder.SetInsertPoint(then_block);

    for (auto &i: *_then_stat)
        i->codegen();

    builder.CreateBr(merge_block);

    function->getBasicBlockList().push_back(else_block);
    builder.SetInsertPoint(else_block);

    if (_else_stat) {
        for (auto &i: *_else_stat)
            i->codegen();
    }

    builder.CreateBr(merge_block);

    function->getBasicBlockList().push_back(merge_block);
    builder.SetInsertPoint(merge_block);
}
void IfStatement::print(int level) const {
    std::cout << prefix(level) << "#if structure: " << std::endl;
    _cond->print(level + 1);
    if (_then_stat) {
        std::cout << prefix(level + 1) << "#then: " << std::endl;
        for (const auto &statement : *_then_stat) {
            statement->print(level + 2);
        }
    }
    if (_else_stat) {
        std::cout << prefix(level + 1) << "#else: " << std::endl;
        for (const auto &statement : *_else_stat) {
            statement->print(level + 2);
        }
    }
}