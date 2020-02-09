#pragma once
#include <utility>
#include <vector>
#include <string>

#include "astTree/astTree.hpp"
#include "expression/expr.hpp"

#include <llvm/IR/Value.h>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <iostream>
#include "alloc/alloc.hpp"

//extern llvm::LLVMContext context;
//extern std::map<std::string, llvm::AllocaInst *> named_values;
//extern llvm::IRBuilder<> builder;
//extern llvm::Module *module;
//llvm::Value *Str;

//extern void yyerror(std::string msg);

class Statement {
 public:
  virtual ~Statement() = default;
  virtual void print(int = 0) const = 0;
  virtual void codegen() = 0;
};

class FunctionPrototypeAST {
 public:
  FunctionPrototypeAST(std::string name, std::vector<Param *> params, VarType return_type) :
      m_name(std::move(name)), m_params(std::move(params)), m_ret_type(return_type) {};
  llvm::Function *codegen();

  [[nodiscard]] const std::string &getId() const {
    return m_name;
  }
  void print(int) const;
 private:
  std::string m_name;
  std::vector<Param *> m_params;
  VarType m_ret_type;
};

class FunctionAST : public Statement {
 public:
  FunctionAST(FunctionPrototypeAST *prototype, std::vector<Statement *> body) :
      m_prototype(prototype), m_block(std::move(body)) {
  };
  void codegen() override;
  void print(int) const override;

  ~FunctionAST() override;

 private:
  FunctionPrototypeAST *m_prototype;
  std::vector<Statement *> m_block;
};

class ExpressionStatement : public Statement {
 public:
  explicit ExpressionStatement(Expression *expr) : expr(expr) {};

  void codegen() override {
    expr->codegen();
  }
  void print(int) const override;

  ~ExpressionStatement() override {
    delete expr;
  }
 private:
  Expression *expr;
};

class ReturnStatement : public Statement {
 public:
  explicit ReturnStatement(Expression *expr) : _expr(expr) {};
  void print(int) const override;
  void codegen() override;
 private:
  Expression *_expr;
};

class EmptyStatement : public Statement {
  void codegen() override {}
  void print(int) const override {};
};

class VarDeclarationStatement : public Statement {
 public:
  VarDeclarationStatement(std::string id, VarType type) :
      m_id(std::move(id)), m_type(type) {};
  void codegen() override;
  void print(int) const override;
  [[nodiscard]] const std::string &getId() const {
    return m_id;
  }
 private:
  std::string m_id;
  VarType m_type;
};

class AssignStatement : public Statement {
 public:
  AssignStatement(std::string id, Expression *expr) : _id(std::move(id)), _expr(expr) {};
  void codegen() override;
  void print(int) const override;

  ~AssignStatement() override {
    delete _expr;
  }
 private:
  std::string _id;
  Expression *_expr;
};

class DeclareAndAssignStatement : public Statement {
 public:
  DeclareAndAssignStatement(VarDeclarationStatement *decl_statement, AssignStatement *assign_statement)
      : _decl_statement(decl_statement), _assign_statement(assign_statement) {};

  void codegen() override;
  void print(int) const override;

  ~DeclareAndAssignStatement() override {
    delete _decl_statement;
    delete _assign_statement;
  }
 private:
  VarDeclarationStatement *_decl_statement;
  AssignStatement *_assign_statement;
};

class IfStatement : public Statement {
 public:
  IfStatement(Expression *cond, std::vector<Statement *> *then_stat, std::vector<Statement *> *else_stat = nullptr)
      : _cond(cond), _then_stat(then_stat), _else_stat(else_stat) {};
  void codegen() override;
  void print(int) const override;

  ~IfStatement() override {
    delete _cond;
    for (auto &i : *_then_stat)
      delete i;
    for (auto &i : *_else_stat)
      delete i;
  }

 private:
  Expression *_cond;
  std::vector<Statement *> *_then_stat;
  std::vector<Statement *> *_else_stat;
};

class PrintStatement : public Statement {
 public:
  explicit PrintStatement(Expression *e);
  void codegen() override;
  void print(int) const override;

  ~PrintStatement() override {
    delete _e;
  }

 private:
  inline static llvm::Function *m_print = nullptr;
  Expression *_e;
};

class WhileStatement : public Statement {
 public:
  WhileStatement(Expression *cond, std::vector<Statement *> *then_stat)
      : _cond(cond), _then_stat(then_stat) {};
  void codegen() override;
  void print(int) const override;

  ~WhileStatement() override {
    delete _cond;
    for (auto &i : *_then_stat)
      delete i;
  }

 private:
  Expression *_cond;
  std::vector<Statement *> *_then_stat;
};

class ForStatement : public Statement {
 public:
  ForStatement(Statement *init, Expression *cond, Statement *inc, std::vector<Statement *> *block)
      : m_init(init), m_cond(cond), m_inc(inc), _block(block) {};
  void codegen() override;
  void print(int) const override;

  ~ForStatement() override {
    for (auto &i : *_block)
      delete i;
  }
 private:
  Statement *m_init;
  Expression *m_cond;
  Statement *m_inc;
  std::vector<Statement *> *_block;
};
