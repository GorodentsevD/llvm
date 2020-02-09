#include "expr.hpp"

llvm::Value *NotLExpression::codegen() {
    llvm::Value *value_first = m_operand->codegen();
    if (value_first == nullptr) {
        yyerror("Error");
    }
    return llvm::ConstantInt::get(context, llvm::APInt(1, value_first ? 0 : 1));
}
void NotLExpression::print(int level) const {
    std::cout << prefix(level) << "NOT  " << std::endl;
    m_operand->print(level + 1);
}