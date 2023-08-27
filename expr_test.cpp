#include "expr.hpp"
#include <iostream>

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
            variables[var_ind] = it->second;
            symbol_table.add_variable(it->first, variables[var_ind]);
            std::cerr << "name: " << it->first << " value: " << variables[var_ind] << std::endl;
            var_ind++;
        }
    }

    std::cerr << "here" << std::endl;
    symbol_table.add_constants();
    std::cerr << "here" << std::endl;
    expression_t expression;
    std::cerr << "here" << std::endl;
    expression.register_symbol_table(symbol_table);
    std::cerr << "here" << std::endl;

    parser_t parser;
    std::cerr << "here" << std::endl;
    parser.compile(expression_string, expression);
    std::cerr << "here" << std::endl;
    std::cerr << "expr string: " << expr << std::endl;
    std::cerr << "expr value : " << expression.value() << std::endl;
    return expression.value();
}