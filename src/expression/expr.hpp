#pragma once

#include <utility>
#include <vector>
#include <string>
#include <variant>

#include <llvm/IR/Value.h>
#include "astTree/astTree.hpp"

#include <iostream>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

extern llvm::LLVMContext context;
extern std::map<std::string, llvm::AllocaInst *> named_values;
extern llvm::IRBuilder<> builder;
extern llvm::Module *module;
extern void yyerror(std::string msg);


enum class LogicOperator {
  LESS,
  GREATER,
  LESS_EQUAL,
  GREATER_EQUAL,
  EQUAL,
  NOT_EQUAL
};

std::string logic_operator_to_string(LogicOperator);

class Expression {
 public:
  virtual ~Expression() = default;
  virtual void print(int = 0) const = 0;
  virtual llvm::Value *codegen() = 0;
};

class Literal : public Expression {
 public:
  explicit Literal(std::variant<std::string, int> value) : m_lit{std::move(value)} {}
  void print(int) const override;
  llvm::Value *codegen() override;
 private:
  std::variant<std::string, int> m_lit;
};

class IdentifierExpression : public Expression {
 public:
  llvm::Value *codegen() override;
  void print(int) const override;
  explicit IdentifierExpression(std::string id) : m_id(std::move(id)) {}
 private:
  std::string m_id;
};

class UnaryExpression : public Expression {
 public:
  explicit UnaryExpression(Expression *operand)
      : m_operand(operand) {};
  ~UnaryExpression() override;
 protected:
  Expression *m_operand;
};

class NotLExpression : public UnaryExpression {
 public:
  explicit NotLExpression(Expression *first) : UnaryExpression(first) {};
  void print(int) const override;
  llvm::Value *codegen() override;
};

class BinaryExpression : public Expression {
 public:
  BinaryExpression(Expression *l, Expression *r)
      : m_left(l), m_right(r) {};
  ~BinaryExpression() override;
 protected:
  Expression *m_left;
  Expression *m_right;
};

class MathBinaryExpression : public BinaryExpression {
 public:
  MathBinaryExpression(Expression *l, char op, Expression *r)
      : BinaryExpression(l, r),
        m_op{op} {};
  void print(int) const override;
  llvm::Value *codegen() override;
 private:
  char m_op;
};

class LogicBinaryExpression : public BinaryExpression {
 public:
  LogicBinaryExpression(Expression *l, LogicOperator op, Expression *r)
      : BinaryExpression(l, r),
        m_op{op} {};
  void print(int) const override;
  llvm::Value *codegen() override;
 private:
  LogicOperator m_op;
};

class AndExpression : public BinaryExpression {
 public:
  AndExpression(Expression *l, Expression *r)
      : BinaryExpression(l, r) {};
  void print(int) const override;
  llvm::Value *codegen() override;
};

class OrExpression : public BinaryExpression {
 public:
  OrExpression(Expression *l, Expression *r)
      : BinaryExpression(l, r) {};
  void print(int) const override;
  llvm::Value *codegen() override;
};

class FunctionCallExpression : public Expression {
 public:
  explicit FunctionCallExpression(std::string func, std::vector<Expression *> args)
      : m_function(std::move(func)),
        m_args(std::move(args)) {};
  void print(int) const override;
  llvm::Value *codegen() override;
 private:
  std::string m_function;
  std::vector<Expression *> m_args;
};