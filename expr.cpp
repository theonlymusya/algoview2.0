#include "expr.hpp"
#include <iostream>
#include "logger.hpp"
#include "output_file_manager.hpp"

const std::string file_name = "expr.cpp";

namespace reg_expr {
using namespace logger;
using namespace output_file_manager;

double calc_expr(std::string expr, const std::map<std::string, double>& variables_values_map) {
    const std::string func_name = "calc_expr";
    auto& logger = Logger::get_instance();
    logger.log_file_enter(func_name, file_name);
    logger.log_info_msg("Start calculating expression : " + expr);
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
    auto result = expression.value();
    if (result != result && !expr.empty()) {
        std::string msg = "Incorrect string or symbol in expression: " + expr;
        logger.log_err_msg(func_name, file_name, msg);
        logger.add_user_error(msg);
        auto& output_file = OutputFileManager::get_instance();
        output_file.fatal_error_report();
        exit(1);
    }

    logger.log_file_exit(func_name, file_name);
    return result;
}
}  // namespace reg_expr