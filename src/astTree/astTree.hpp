#pragma once
#include <utility>
#include <vector>
#include <string>
#include <variant>

#include <llvm/IR/Value.h>

enum class VarType {
  INT,
  STRING
};

inline std::string prefix(int level, const char prefix_value = ' ', const uint prefix_len = 4) {
  std::string r;
  std::fill_n(std::back_insert_iterator(r), level * 2, prefix_value);
  return r;
}

std::string type_to_string(VarType type);
llvm::Type *type_to_llvm_type(VarType type);

class Param {
 public:
  Param(std::string id, VarType type) : m_id(std::move(id)), m_type(type) {};
  void print(int) const;

  [[nodiscard]] const std::string &getId() const {
    return m_id;
  }

  [[nodiscard]] VarType getType() const {
    return m_type;
  }
 private:
  std::string m_id;
  VarType m_type;
};
