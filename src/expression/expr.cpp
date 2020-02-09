#include "expr.hpp"
#include <iostream>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

//extern llvm::LLVMContext context;
//extern std::map<std::string, llvm::AllocaInst *> named_values;
//extern llvm::IRBuilder<> builder;
//extern llvm::Module *module;

//extern void yyerror(std::string msg);

//std::string logic_operator_to_string(LogicOperator op) {
//  switch (op) {
//    case LogicOperator::EQUAL: return "EQUAL";
//    case LogicOperator::NOT_EQUAL: return "NOT_EQUAL";
//    case LogicOperator::LESS: return "LESS";
//    case LogicOperator::GREATER: return "GREATER";
//    case LogicOperator::LESS_EQUAL: return "LESS_EQUAL";
//    case LogicOperator::GREATER_EQUAL: return "GREATER_EQUAL";
//  }
//}
/*
 =========
  LITERAL
 =========
 */
//llvm::Value *Literal::codegen() {
//  if (auto int_ptr = std::get_if<int>(&m_lit)) {
//    return llvm::ConstantInt::get(context, llvm::APInt(32, *int_ptr));
//  } else if (auto str_ptr = std::get_if<std::string>(&m_lit)) {
//    return builder.CreateGlobalStringPtr(llvm::StringRef(*str_ptr));
//  } else {
//    return nullptr;
//  }
//}
//void Literal::print(int level) const {
//  if (auto int_ptr = std::get_if<int>(&m_lit)) {
//    std::cout << prefix(level) << "INT " << *int_ptr << std::endl;
//  } else {
//    std::cout << prefix(level) << "STRING " << std::get<std::string>(m_lit) << std::endl;
//  }
//}

/*
 ============
  IDENTIFIER
 ============
 */
//llvm::Value *IdentifierExpression::codegen() {
//  llvm::Value *value = named_values[m_id];
//  if (value == nullptr) {
//    std::cerr << "Unknown variable name: " << m_id << std::endl;
//    exit(EXIT_FAILURE);
//  }
//  return builder.CreateLoad(value, m_id);
//}
//void IdentifierExpression::print(int level) const {
//  std::cout << prefix(level) << "ID: " << m_id << std::endl;
//}

/*
 ==================
  UNARY EXPRESSION
 ==================
 */
//UnaryExpression::~UnaryExpression() {
//  delete m_operand;
//}

/*
 =======
  ERROR
 =======
 */
//llvm::Value *NotLExpression::codegen() {
//  llvm::Value *value_first = m_operand->codegen();
//  if (value_first == nullptr) {
//    yyerror("Error");
//  }
//  return llvm::ConstantInt::get(context, llvm::APInt(1, value_first ? 0 : 1));
//}
//void NotLExpression::print(int level) const {
//  std::cout << prefix(level) << "NOT  " << std::endl;
//  m_operand->print(level + 1);
//}

/*
 ====================
  BINARY EXPRESSIONS
 ====================
 */
//BinaryExpression::~BinaryExpression() {
//  delete m_left;
//  delete m_right;
//}

/*
 =====================
  MATH BIN EXPRESSION
 =====================
 */
//llvm::Value *MathBinaryExpression::codegen() {
//  llvm::Value *value_first = m_left->codegen();
//  llvm::Value *value_second = m_right->codegen();
//  if (value_first == nullptr || value_second == nullptr) {
//    yyerror("Error");
//    return nullptr;
//  }
//  if (!(value_first->getType() == llvm::Type::getInt32Ty(context) &&
//      value_second->getType() == llvm::Type::getInt32Ty(context))) {
//    yyerror("Error type");
//    return nullptr;
//  }
//  switch (m_op) {
//    case '+': return builder.CreateAdd(value_first, value_second, "addtmp");
//    case '-': return builder.CreateSub(value_first, value_second, "subtmp");
//    case '*': return builder.CreateMul(value_first, value_second, "multmp");
//    case '/': return builder.CreateSDiv(value_first, value_second, "divtmp");
//    default: yyerror("Error type");
//      return nullptr;
//  }
//}
//void MathBinaryExpression::print(int level) const {
//  std::cout << prefix(level) << "#bin operation: " << std::endl;
//  m_left->print(level + 1);
//  std::cout << prefix(level + 1) << "#type: " << m_op << std::endl;
//  m_right->print(level + 1);
//  std::cout << prefix(level) << std::endl;
//}

/*
 ======================
  LOGIC BIN EXPRESSION
 ======================
 */
//llvm::Value *LogicBinaryExpression::codegen() {
//  llvm::Value *value_first = m_left->codegen();
//  llvm::Value *value_second = m_right->codegen();
//  if (value_first == nullptr || value_second == nullptr) {
//    yyerror("Error");
//    return nullptr;
//  }
//  switch (m_op) {
//    case LogicOperator::EQUAL: return builder.CreateICmpEQ(value_first, value_second, "eqtmp");
//    case LogicOperator::NOT_EQUAL: return builder.CreateICmpNE(value_first, value_second, "netmp");
//    case LogicOperator::LESS: return builder.CreateICmpSLT(value_first, value_second, "lesstmp");
//    case LogicOperator::GREATER: return builder.CreateICmpSGT(value_first, value_second, "grttmp");
//    case LogicOperator::LESS_EQUAL: return builder.CreateICmpSLE(value_first, value_second, "leetmp");
//    case LogicOperator::GREATER_EQUAL: return builder.CreateICmpSGE(value_first, value_second, "geetmp");
//    default: yyerror("Invalid operator");
//      return nullptr;
//  }
//}
//void LogicBinaryExpression::print(int level) const {
//  std::cout << prefix(level) << "#logic operation: " << std::endl;
//  m_left->print(level + 1);
//  std::cout << prefix(level + 1) << "#type: " << logic_operator_to_string(m_op) << std::endl;//todo: enum -> string
//  m_right->print(level + 1);
//}

/*
 ================
  AND EXPRESSION
 ================
 */
//llvm::Value *AndExpression::codegen() {
//  llvm::Value *value_first = m_left->codegen();
//  llvm::Value *value_second = m_right->codegen();
//  if (value_first == nullptr || value_second == nullptr) {
//    yyerror("Error");
//    return nullptr;
//  }
//  if (value_first->getType() != llvm::Type::getInt1Ty(context)
//      || value_second->getType() != llvm::Type::getInt1Ty(context)) {
//    yyerror("Must be boolean");
//    return nullptr;
//  }
//  llvm::Value *lhs = llvm::ConstantInt::get(context, llvm::APInt(1, value_first ? 1 : 0));
//  llvm::Value *rhs = llvm::ConstantInt::get(context, llvm::APInt(1, value_second ? 1 : 0));
//  return llvm::ConstantInt::get(context, llvm::APInt(1, lhs && rhs));
//}
//void AndExpression::print(int level) const {
//  std::cout << prefix(level) << "#binary operation:  " << std::endl;
//  m_left->print(level + 1);
//  std::cout << prefix(level + 1) << "#type: AND" << std::endl;
//  m_right->print(level + 1);
//}

/*
 ===============
  OR EXPRESSION
 ===============
 */
//llvm::Value *OrExpression::codegen() {
//  llvm::Value *value_first = m_left->codegen();
//  llvm::Value *value_second = m_right->codegen();
//  if (value_first == nullptr || value_second == nullptr) {
//    yyerror("Error");
//    return nullptr;
//  }
//  if (value_first->getType() != llvm::Type::getInt1Ty(context)
//      || value_second->getType() != llvm::Type::getInt1Ty(context)) {
//    yyerror("Must be boolean");
//    return nullptr;
//  }
//  llvm::Value *lhs = builder.CreateICmpEQ(value_first, llvm::ConstantInt::get(context, llvm::APInt(8, 1)), "ifcondl");
//  llvm::Value *rhs = builder.CreateICmpEQ(value_first, llvm::ConstantInt::get(context, llvm::APInt(8, 1)), "ifcondl");
//  return llvm::ConstantInt::get(context, llvm::APInt(1, !(lhs && rhs)));
//}
//void OrExpression::print(int level) const {
//  std::cout << prefix(level) << "#binary operation:" << std::endl;
//  m_left->print(level + 1);
//  std::cout << prefix(level + 1) << "#type: OR" << std::endl;
//  m_right->print(level + 1);
//}

/*
 ======================
  FUNC CALL EXPRESSION
 ======================
 */
//llvm::Value *FunctionCallExpression::codegen() {
//  llvm::Function *callee_function = module->getFunction(m_function);
//  if (callee_function == nullptr) {
//    yyerror("Function " + m_function + " doesn't exist");
//    return nullptr;
//  }
//
//  unsigned arg_size = callee_function->arg_size();
//
//  if (arg_size != m_args.size()) {
//    yyerror("Wrong number of arguments: " + m_function);
//    return nullptr;
//  }
//
//  std::vector<llvm::Value *> generated_args;
//  for (unsigned i = 0; i < arg_size; ++i) {
//    llvm::Value *arg_value = m_args[i]->codegen();
//    if (arg_value == nullptr) {
//      return nullptr;
//    }
//    generated_args.push_back(arg_value);
//  }
//
//  return builder.CreateCall(callee_function, generated_args, "calltmp");
//}
//void FunctionCallExpression::print(int level) const {
//  std::cout << prefix(level) << "#call:";
//  std::cout << prefix(level + 1) << "function: " << m_function << std::endl;
//  std::cout << prefix(level + 1) << "arguments: " << std::endl;
//  for (const auto& arg : m_args) {
//    arg->print(level + 1);
//  }
//}