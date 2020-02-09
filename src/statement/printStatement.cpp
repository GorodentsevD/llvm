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
 PRINT STATEMENT
=================
*/
PrintStatement::PrintStatement(Expression *e) : _e(e) {
    if (m_print == nullptr) {
        llvm::FunctionType *FT1 =
                llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(context),
                                        llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), true);
        m_print = llvm::Function::Create(FT1, llvm::Function::ExternalLinkage, "printf", module);
    }
}
void PrintStatement::codegen() {
    llvm::Value *l = _e->codegen();
    if (l == nullptr)
        return;

    if (l->getType() != llvm::Type::getInt8PtrTy(context))
        Str = builder.CreateGlobalStringPtr("%d\n");
    else
        Str = builder.CreateGlobalStringPtr("%s\n");

    std::vector<llvm::Value *> ArgsV;
    ArgsV.push_back(Str);
    ArgsV.push_back(l);
    builder.CreateCall(m_print, ArgsV, "println");
}
void PrintStatement::print(int level) const {
    std::cout << prefix(level) << "#print:";
    _e->print(level);
}