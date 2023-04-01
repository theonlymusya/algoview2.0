#include <iostream>
#include "expr.hpp"

using CoordType = int;
using namespace reg_expr;
std::map<std::string, double> args_map;

void parse_value(const std::string& str, std::string& left_value, std::string& right_value, char parse_sym) {
    int first_parse_sym_pos = str.find(parse_sym);
    char* c_left_value = (char*)malloc(first_parse_sym_pos);
    int left_value_len = str.copy(c_left_value, first_parse_sym_pos, 0);
    c_left_value[left_value_len] = '\0';
    left_value = std::string(c_left_value);
    if (parse_sym == ',')
        first_parse_sym_pos++;
    else if (parse_sym == '.')
        first_parse_sym_pos += 2;
    int right_value_len = str.length() - first_parse_sym_pos;
    char* c_right_value = (char*)malloc(right_value_len);
    left_value_len = str.copy(c_right_value, right_value_len, first_parse_sym_pos);
    c_right_value[left_value_len] = '\0';
    right_value = std::string(c_right_value);
}

void get_src_vertex_coords(const std::string& src_string, int dim, CoordType& i, CoordType& j, CoordType& k) {
    std::string i_str, j_str, k_str, jk_str;
    if (dim == 1) {
        i = calc_expr(src_string, args_map);
        j = -1;
        k = -1;
    } else if (dim == 2) {
        parse_value(src_string, i_str, j_str, ',');
        i = calc_expr(i_str, args_map);
        j = calc_expr(j_str, args_map);
        k = -1;
    } else if (dim == 3) {
        parse_value(src_string, i_str, jk_str, ',');
        parse_value(jk_str, j_str, k_str, ',');
        i = calc_expr(i_str, args_map);
        j = calc_expr(j_str, args_map);
        k = calc_expr(k_str, args_map);
    }
}

int main() {
    args_map["i"] = 4;
    args_map["j"] = 5;
    args_map["k"] = 2;
    args_map["p1"] = 1;
    args_map["p2"] = 1;
    CoordType src_i, src_j, src_k;
    get_src_vertex_coords("i + p1, j + p2, k", 3, src_i, src_j, src_k);
    std::cout << "i = " << src_i << "\nj = " << src_j << "\nk = " << src_k << std::endl;
    get_src_vertex_coords("i + p1, j + p2", 2, src_i, src_j, src_k);
    std::cout << "i = " << src_i << "\nj = " << src_j << "\nk = " << src_k << std::endl;
    get_src_vertex_coords("i + p1", 1, src_i, src_j, src_k);
    std::cout << "i = " << src_i << "\nj = " << src_j << "\nk = " << src_k << std::endl;
}
