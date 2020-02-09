#include "expr.hpp"

/*
 =========
  LITERAL
 =========
 */
llvm::Value *Literal::codegen() {
    if (auto int_ptr = std::get_if<int>(&m_lit)) {
        return llvm::ConstantInt::get(context, llvm::APInt(32, *int_ptr));
    } else if (auto str_ptr = std::get_if<std::string>(&m_lit)) {
        return builder.CreateGlobalStringPtr(llvm::StringRef(*str_ptr));
    } else {
        return nullptr;
    }
}
void Literal::print(int level) const {
  if (auto int_ptr = std::get_if<int>(&m_lit)) {
    std::cout << prefix(level) << "INT " << *int_ptr << std::endl;
  } else {
    std::cout << prefix(level) << "STRING " << std::get<std::string>(m_lit) << std::endl;
  }
}
