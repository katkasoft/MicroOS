#include <string>
#include <sstream>
#include <stdexcept>

double calc(std::string expr) {
    std::istringstream iss(expr);
    double result = 0.0;
    double current_number = 0.0;
    char op = '+';
    char next_op;
    while (iss >> current_number) {
        if (op == '+') {
            result += current_number;
        } else if (op == '-') {
            result -= current_number;
        } else if (op == '*') {
            double temp = current_number;
            if (!(iss >> next_op)) {
                result *= temp;
                break;
            }
            if (next_op == '*' || next_op == '/') {
                while (next_op == '*' || next_op == '/') {
                    double next_num;
                    iss >> next_num;                    
                    if (next_op == '*') {
                        temp *= next_num;
                    } else {
                        if (next_num == 0) throw std::runtime_error("Division by zero");
                        temp /= next_num;
                    }     
                    if (!(iss >> next_op)) break;
                }                
                if (op == '+') {
                    result += temp;
                } else {
                    result -= temp;
                }                
                if (!(iss)) {
                    break;
                }
                op = next_op;
                continue;
            } else {
                result *= temp;
                op = next_op;
                continue;
            }
        } else if (op == '/') {
            double temp = current_number;
            if (current_number == 0) throw std::runtime_error("Division by zero");
            if (!(iss >> next_op)) {
                result /= temp;
                break;
            }
            if (next_op == '*' || next_op == '/') {
                while (next_op == '*' || next_op == '/') {
                    double next_num;
                    iss >> next_num;       
                    if (next_op == '*') {
                        temp *= next_num;
                    } else {
                        if (next_num == 0) throw std::runtime_error("Division by zero");
                        temp /= next_num;
                    }
                    if (!(iss >> next_op)) break;
                }
                if (op == '+') {
                    result += temp;
                } else {
                    result -= temp;
                }
                if (!(iss)) {
                    break;
                }
                op = next_op;
                continue;
            } else {
                result /= temp;
                op = next_op;
                continue;
            }
        }
        if (!(iss >> op)) {
            break;
        }
    }
    return result;
}