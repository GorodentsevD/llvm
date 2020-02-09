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
============
 EXPRESSION
============
*/
void ExpressionStatement::print(int level) const {
  if (expr) {
    expr->print(level);
  }
}

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
}

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
    std::cout << prefix(level) << "#print:" << std::endl;
    _e->print(level + 1);
}

/*
===============
 FOR STATEMENT
===============
*/
void ForStatement::codegen() {
    llvm::Function *function = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock *cond_block = llvm::BasicBlock::Create(context, "cond");
    llvm::BasicBlock *loop_block = llvm::BasicBlock::Create(context, "loop");
    llvm::BasicBlock *after_loop_block = llvm::BasicBlock::Create(context, "afterloop");

    if (m_init) {
        m_init->codegen();
    }
    builder.CreateBr(cond_block);

    function->getBasicBlockList().push_back(cond_block);
    builder.SetInsertPoint(cond_block);

    llvm::Value *loop_cond_value = m_cond->codegen();

    builder.CreateCondBr(loop_cond_value, loop_block, after_loop_block);

    function->getBasicBlockList().push_back(loop_block);
    builder.SetInsertPoint(loop_block);

    for (auto &i: *_block)
        i->codegen();

    if (m_inc) {
        m_inc->codegen();
    }
    builder.CreateBr(cond_block);

    function->getBasicBlockList().push_back(after_loop_block);
    builder.SetInsertPoint(after_loop_block);
}
void ForStatement::print(int level) const {
    std::cout << std::endl << prefix(level) << "#for structure: " << std::endl;
    if (m_init) {
        std::cout << prefix(level + 1) << "initialize: " << std::endl;
        m_init->print(level + 2);
    }
    std::cout << prefix(level + 1) << "condition: " << std::endl;
    if (m_inc) {
        std::cout << prefix(level + 1) << "iter: " << std::endl;
        m_inc->print(level + 2);
    }
    if (_block) {
        std::cout << prefix(level + 1) << "body: " << std::endl;
        for (const auto &statement : *_block) {
            statement->print(level + 2);
        }
    }
}

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
    std::cout << prefix(level + 1) << "body:" << std::endl;
    for (const auto &statement : m_block) {
        statement->print(level + 2);
    }
}





