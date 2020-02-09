#include "expr.hpp"

BinaryExpression::~BinaryExpression() {
    delete m_left;
    delete m_right;
}

/*
 =====================
  MATH BIN EXPRESSION
 =====================
 */
llvm::Value *MathBinaryExpression::codegen() {
    llvm::Value *value_first = m_left->codegen();
    llvm::Value *value_second = m_right->codegen();
    if (value_first == nullptr || value_second == nullptr) {
        yyerror("Error");
        return nullptr;
    }
    if (!(value_first->getType() == llvm::Type::getInt32Ty(context) &&
          value_second->getType() == llvm::Type::getInt32Ty(context))) {
        yyerror("Error type");
        return nullptr;
    }
    switch (m_op) {
        case '+': return builder.CreateAdd(value_first, value_second, "addtmp");
        case '-': return builder.CreateSub(value_first, value_second, "subtmp");
        case '*': return builder.CreateMul(value_first, value_second, "multmp");
        case '/': return builder.CreateSDiv(value_first, value_second, "divtmp");
        default: yyerror("Error type");
            return nullptr;
    }
}
void MathBinaryExpression::print(int level) const {
    std::cout << prefix(level) << "#bin operation: " << std::endl;
    m_left->print(level + 1);
    std::cout << prefix(level + 1) << "#type: " << m_op << std::endl;
    m_right->print(level + 1);
    std::cout << prefix(level) << std::endl;
}

/*
 ======================
  LOGIC BIN EXPRESSION
 ======================
 */
std::string logic_operator_to_string(LogicOperator op) {
    switch (op) {
        case LogicOperator::EQUAL: return "EQUAL";
        case LogicOperator::NOT_EQUAL: return "NOT_EQUAL";
        case LogicOperator::LESS: return "LESS";
        case LogicOperator::GREATER: return "GREATER";
        case LogicOperator::LESS_EQUAL: return "LESS_EQUAL";
        case LogicOperator::GREATER_EQUAL: return "GREATER_EQUAL";
    }
}
llvm::Value *LogicBinaryExpression::codegen() {
    llvm::Value *value_first = m_left->codegen();
    llvm::Value *value_second = m_right->codegen();
    if (value_first == nullptr || value_second == nullptr) {
        yyerror("Error");
        return nullptr;
    }
    switch (m_op) {
        case LogicOperator::EQUAL: return builder.CreateICmpEQ(value_first, value_second, "eqtmp");
        case LogicOperator::NOT_EQUAL: return builder.CreateICmpNE(value_first, value_second, "netmp");
        case LogicOperator::LESS: return builder.CreateICmpSLT(value_first, value_second, "lesstmp");
        case LogicOperator::GREATER: return builder.CreateICmpSGT(value_first, value_second, "grttmp");
        case LogicOperator::LESS_EQUAL: return builder.CreateICmpSLE(value_first, value_second, "leetmp");
        case LogicOperator::GREATER_EQUAL: return builder.CreateICmpSGE(value_first, value_second, "geetmp");
        default: yyerror("Invalid operator");
            return nullptr;
    }
}
void LogicBinaryExpression::print(int level) const {
    std::cout << prefix(level) << "#logic operation: " << std::endl;
    m_left->print(level + 1);
    std::cout << prefix(level + 1) << "#type: " << logic_operator_to_string(m_op) << std::endl;//todo: enum -> string
    m_right->print(level + 1);
}

/*
 ================
  AND EXPRESSION
 ================
 */
llvm::Value *AndExpression::codegen() {
    llvm::Value *value_first = m_left->codegen();
    llvm::Value *value_second = m_right->codegen();
    if (value_first == nullptr || value_second == nullptr) {
        yyerror("Error");
        return nullptr;
    }
    if (value_first->getType() != llvm::Type::getInt1Ty(context)
        || value_second->getType() != llvm::Type::getInt1Ty(context)) {
        yyerror("Must be boolean");
        return nullptr;
    }
    llvm::Value *lhs = llvm::ConstantInt::get(context, llvm::APInt(1, value_first ? 1 : 0));
    llvm::Value *rhs = llvm::ConstantInt::get(context, llvm::APInt(1, value_second ? 1 : 0));
    return llvm::ConstantInt::get(context, llvm::APInt(1, lhs && rhs));
}
void AndExpression::print(int level) const {
    std::cout << prefix(level) << "#binary operation:  " << std::endl;
    m_left->print(level + 1);
    std::cout << prefix(level + 1) << "#type: AND" << std::endl;
    m_right->print(level + 1);
}

/*
 ===============
  OR EXPRESSION
 ===============
 */
llvm::Value *OrExpression::codegen() {
    llvm::Value *value_first = m_left->codegen();
    llvm::Value *value_second = m_right->codegen();
    if (value_first == nullptr || value_second == nullptr) {
        yyerror("Error");
        return nullptr;
    }
    if (value_first->getType() != llvm::Type::getInt1Ty(context)
        || value_second->getType() != llvm::Type::getInt1Ty(context)) {
        yyerror("Must be boolean");
        return nullptr;
    }
    llvm::Value *lhs = builder.CreateICmpEQ(value_first, llvm::ConstantInt::get(context, llvm::APInt(8, 1)), "ifcondl");
    llvm::Value *rhs = builder.CreateICmpEQ(value_first, llvm::ConstantInt::get(context, llvm::APInt(8, 1)), "ifcondl");
    return llvm::ConstantInt::get(context, llvm::APInt(1, !(lhs && rhs)));
}
void OrExpression::print(int level) const {
    std::cout << prefix(level) << "#binary operation:" << std::endl;
    m_left->print(level + 1);
    std::cout << prefix(level + 1) << "#type: OR" << std::endl;
    m_right->print(level + 1);
}