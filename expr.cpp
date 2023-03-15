#include "expr.hpp"

namespace reg_expr {
double calc_expr(std::string expr, const std::map<std::string, double>& variables_values_map) {
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double> parser_t;

    const std::string expression_string(expr);
    std::vector<double> variables(variables_values_map.size());

    symbol_table_t symbol_table;

    int var_ind = 0;
    for (auto it = variables_values_map.cbegin(); it != variables_values_map.cend(); ++it) {
        if (expr.find(it->first) != std::string::npos) {
            symbol_table.add_variable(it->first, variables[var_ind]);
            variables[var_ind] = it->second;
            var_ind++;
        }
    }

    symbol_table.add_constants();

    expression_t expression;
    expression.register_symbol_table(symbol_table);

    parser_t parser;
    parser.compile(expression_string, expression);

    return expression.value();
}
}  // namespace reg_expr