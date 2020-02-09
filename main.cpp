#include <iostream>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

extern FILE *yyin;
int yyparse();

llvm::LLVMContext context;
llvm::IRBuilder<> builder(context);
llvm::Module *module;
std::map<std::string, llvm::AllocaInst *> named_values;

int main() {
  yyin = fopen("../main.tb", "r");
  module = new llvm::Module("My module", context);

  /* Start analyze */
  yyparse();

  std::error_code err;
  llvm::raw_fd_ostream file("../main.ll", err);
  if (err) {
    std::cerr << err.message() << std::endl;
    return 1;
  }
  module->print(file, nullptr);
  file.flush();


  delete module;
  return 0;
}