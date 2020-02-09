#include "expr.hpp"

UnaryExpression::~UnaryExpression() {
    delete m_operand;
}