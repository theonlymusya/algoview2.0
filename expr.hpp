#pragma once
#include "exprtk.hpp"

namespace reg_expr {
double calc_expr(std::string expr, const std::map<std::string, double>& variables_values_map);
}  // namespace reg_expr
