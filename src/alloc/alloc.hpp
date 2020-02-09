#pragma once
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "astTree/astTree.hpp"

llvm::AllocaInst* create_entry_block_alloca(llvm::Function* function, const std::string& var_name, llvm::Type* type);
