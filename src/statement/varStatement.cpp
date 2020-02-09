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
   VARIABLE
==============
*/
void VarDeclarationStatement::codegen() {
    llvm::Type *llvm_type = type_to_llvm_type(m_type);
    llvm::AllocaInst *alloca = builder.CreateAlloca(llvm_type, nullptr, m_id);
    named_values[m_id] = alloca;

}
void VarDeclarationStatement::print(int level) const {
    std::cout << prefix(level) << type_to_string(m_type) + " " << "'" +  m_id + "'" << std::endl;
}