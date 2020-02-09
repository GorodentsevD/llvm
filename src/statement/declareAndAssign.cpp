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
 DECLARE AND ASSIGN
====================
*/
void DeclareAndAssignStatement::codegen() {
    _decl_statement->codegen();
    _assign_statement->codegen();
}
void DeclareAndAssignStatement::print(int level) const {
    _decl_statement->print(level);
    _assign_statement->print(level);
}