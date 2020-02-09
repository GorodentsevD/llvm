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
==================
 ASSIGN STATEMENT
==================
*/
void AssignStatement::codegen() {
    llvm::Value *lhs = named_values[_id];
    if (lhs == nullptr) {
        yyerror("ERROR: Unknown variable: " + _id);
    }
    llvm::Value *rhs = _expr->codegen();

    builder.CreateStore(rhs, lhs);
}
void AssignStatement::print(int level) const {
    std::cout << prefix(level) << "#assign: " << _id << std::endl;
    if (_expr) {
        _expr->print(level + 1);
    }
    std::cout << std::endl;
}