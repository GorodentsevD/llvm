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
==============================
 FUNCTION PROTOTYPE STATEMENT
==============================
*/
llvm::Function *FunctionPrototypeAST::codegen() {
    std::vector<llvm::Type *> param_types;

    for (Param *param : m_params) {
        llvm::Type *type = type_to_llvm_type(param->getType());
        param_types.push_back(type);
    }

    llvm::Type *return_type = type_to_llvm_type(m_ret_type);

    llvm::FunctionType *function_type = llvm::FunctionType::get(return_type, param_types, false);

    llvm::Function *function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, m_name, module);

    unsigned i = 0;
    for (auto &param : function->args()) {
        param.setName(m_params[i++]->getId());
    }
    return function;
}
void FunctionPrototypeAST::print(int level) const {
    std::cout << std::endl << prefix(level) << "function_id: " <<  m_name << std::endl;
    std::cout << prefix(level + 1) << "@params: " << std::endl;
    for (const auto &param : m_params) {
        param->print(level + 2);
    }
    std::cout << prefix(level + 1) << "@return: " << type_to_string(m_ret_type) << std::endl;
}