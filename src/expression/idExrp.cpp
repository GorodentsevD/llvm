#include "expr.hpp"

/*
 ============
  IDENTIFIER
 ============
 */
llvm::Value *IdentifierExpression::codegen() {
    llvm::Value *value = named_values[m_id];
    if (value == nullptr) {
        std::cerr << "Unknown variable name: " << m_id << std::endl;
        exit(EXIT_FAILURE);
    }
    return builder.CreateLoad(value, m_id);
}
void IdentifierExpression::print(int level) const {
    std::cout << prefix(level) << "ID: " << m_id << std::endl;
}