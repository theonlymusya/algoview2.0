#include <iostream>
#include "expr.hpp"

// using namespace reg_expr;
using CoordType = double;
using ParamsMap = std::map<std::string, CoordType>;
using ArgsMap = std::map<std::string, CoordType>;
using Map = std::map<std::string, CoordType>;

double calc_expr(std::string expr, std::map<std::string, double>& variables_values_map) {
    // for (const auto& var : variables_values_map) {
    //     assert(expr.find(var.first) == std::string::npos);
    //     std::cout << "Не найдено значение параметра " << var.first << " для вычисления выражения " << expr <<
    //     std::endl;
    // }
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

void create_arg_value_map(const ParamsMap& params_map, ArgsMap& args_map) {
    for (const auto& param : params_map) {
        args_map[param.first] = param.second;
    }
}

void change_arg_value_map(std::string i_name,
                          std::string j_name,
                          std::string k_name,
                          int i_value,
                          int j_value,
                          int k_value,
                          ArgsMap& args_map) {
    args_map[i_name] = i_value;
    args_map[j_name] = j_value;
    args_map[k_name] = k_value;
}

void print_map(const Map& map) {
    std::cout << "\t\t\tCURRENT MAP" << std::endl;
    for (const auto& elem : map) {
        std::cout << elem.first << " = " << elem.second << std::endl;
    }
}

int main() {
    ParamsMap params;
    params["M"] = 0;
    params["N"] = 1;
    params["P"] = 2;
    ArgsMap args;
    create_arg_value_map(params, args);
    print_map(args);
    for (int i = 0; i <= 2; i++)
        for (int j = 0; j <= 2; j++)
            for (int k = 0; k <= 2; k++) {
                change_arg_value_map("a", "b", "c", i, j, k, args);
                print_map(args);
                std::string cond = "(a<=M1)and(b>N)and(c<=P)";
                if (calc_expr(cond, args))
                    std::cout << "CONDITION " << cond << " CORRECT" << std::endl;
            }
}