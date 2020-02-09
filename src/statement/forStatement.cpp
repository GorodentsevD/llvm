#include "statement.hpp"

extern llvm::LLVMContext context;
extern std::map<std::string, llvm::AllocaInst *> named_values;
extern llvm::IRBuilder<> builder;
extern llvm::Module *module;
llvm::Value *Str;

extern void yyerror(std::string msg);

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
        std::cout << prefix(level + 1) << "iteration: " << std::endl;
        m_inc->print(level + 2);
    }
    if (_block) {
        std::cout << prefix(level + 1) << "body: " << std::endl;
        for (const auto &statement : *_block) {
            statement->print(level + 2);
        }
    }
}