#include <iostream>

#include "astTree.hpp"
#include "statement/statement.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

extern llvm::LLVMContext context;
extern std::map<std::string, llvm::AllocaInst *> named_values;
extern llvm::IRBuilder<> builder;
extern llvm::Module *module;

llvm::Type *type_to_llvm_type(VarType type) {
  switch (type) {
    case VarType::INT:return llvm::Type::getInt32Ty(context);
    case VarType::STRING:return llvm::Type::getInt8PtrTy(context);
  }
}

std::string type_to_string(VarType type) {
  switch (type) {
    case VarType::INT:return "INT";
    case VarType::STRING:return "STRING";
  }
}

void ReturnStatement::codegen() {
  llvm::Value *expression_value = _expr->codegen();
  builder.CreateRet(expression_value);
}
void ReturnStatement::print(int level) const {
  std::cout << prefix(level) << "#return: ";
  if (_expr) {
     _expr->print();
  }
}

void Param::print(int level) const {
  std::cout << prefix(level) << type_to_string(m_type) + " " << "'" + m_id + "'"  << std::endl;
}
