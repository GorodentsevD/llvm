#include "expr.hpp"
/*
 ======================
  FUNC CALL EXPRESSION
 ======================
 */
llvm::Value *FunctionCallExpression::codegen() {
    llvm::Function *callee_function = module->getFunction(m_function);
    if (callee_function == nullptr) {
        yyerror("Function " + m_function + " doesn't exist");
        return nullptr;
    }

    unsigned arg_size = callee_function->arg_size();

    if (arg_size != m_args.size()) {
        yyerror("Wrong number of arguments: " + m_function);
        return nullptr;
    }

    std::vector<llvm::Value *> generated_args;
    for (unsigned i = 0; i < arg_size; ++i) {
        llvm::Value *arg_value = m_args[i]->codegen();
        if (arg_value == nullptr) {
            return nullptr;
        }
        generated_args.push_back(arg_value);
    }

    return builder.CreateCall(callee_function, generated_args, "calltmp");
}
void FunctionCallExpression::print(int level) const {
    std::cout << prefix(level) << "#call:" << std::endl;
    std::cout << prefix(level + 1) << "function_id: " << m_function << std::endl;
    std::cout << prefix(level + 1) << "arguments: " << std::endl;
    for (const auto& arg : m_args) {
        arg->print(level + 2);
    }
}