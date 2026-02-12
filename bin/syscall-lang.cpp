#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;

void execute_line(string line) {
    long arg1 = 0; string arg1_temp = ""; bool arg1_ready = false;
    long arg2 = 0; string arg2_temp = ""; bool arg2_ready = false;
    long arg3 = 0; string arg3_temp = ""; bool arg3_ready = false;
    string arg3_string = ""; bool is_arg3_string = false;
    long arg4 = 0; string arg4_temp = ""; bool arg4_ready = false;
    long arg5 = 0; string arg5_temp = ""; bool arg5_ready = false;
    long arg6 = 0; string arg6_temp = ""; bool arg6_ready = false;
    long arg7 = 0; string arg7_temp = ""; bool arg7_ready = false;
    bool quote_opened = false;
    int arg_count = 0;
    
    for (char c : line) {
        if (c == '#' && !quote_opened) {
            break;
        }
        if (c == ' ' && !quote_opened) {
            if (!arg1_ready && !arg1_temp.empty()) {
                arg1 = stol(arg1_temp);
                arg1_ready = true;
                arg_count++;
            } else if (arg1_ready && !arg2_ready && !arg2_temp.empty()) {
                arg2 = stol(arg2_temp);
                arg2_ready = true;
                arg_count++;
            } else if (arg2_ready && !arg3_ready && (!arg3_temp.empty() || is_arg3_string)) {
                if (!is_arg3_string) arg3 = stol(arg3_temp);
                arg3_ready = true;
                arg_count++;
            } else if (arg3_ready && !arg4_ready && !arg4_temp.empty()) {
                arg4 = stol(arg4_temp);
                arg4_ready = true;
                arg_count++;
            } else if (arg4_ready && !arg5_ready && !arg5_temp.empty()) {
                arg5 = stol(arg5_temp);
                arg5_ready = true;
                arg_count++;
            } else if (arg5_ready && !arg6_ready && !arg6_temp.empty()) {
                arg6 = stol(arg6_temp);
                arg6_ready = true;
                arg_count++;
            } else if (arg6_ready && !arg7_ready && !arg7_temp.empty()) {
                arg7 = stol(arg7_temp);
                arg7_ready = true;
                arg_count++;
            }
            continue;
        }
        if (!arg1_ready) {
            if (isdigit(c) || (c == '-' && arg1_temp.empty())) {
                arg1_temp += c;
            } else if (!isspace(c)) {
                cerr << "syscall-lang: error: first argument must be integer!" << endl;
                exit(1);
            }
        } else if (!arg2_ready) {
            if (isdigit(c) || (c == '-' && arg2_temp.empty())) {
                arg2_temp += c;
            } else if (!isspace(c)) {
                cerr << "syscall-lang: error: second argument must be integer!" << endl;
                exit(1);
            }
        } else if (!arg3_ready) {
            if (c == '"') {
                if (quote_opened) {
                    arg3_string = arg3_temp;
                    arg3_temp = "";
                    is_arg3_string = true;
                }
                quote_opened = !quote_opened;
                continue;
            }
            arg3_temp += c;
        } else if (!arg4_ready) {
            if (isdigit(c) || (c == '-' && arg4_temp.empty())) {
                arg4_temp += c;
            } else if (!isspace(c)) {
                cerr << "syscall-lang: error: fourth argument must be integer!" << endl;
                exit(1);
            }
        } else if (!arg5_ready) {
            if (isdigit(c) || (c == '-' && arg5_temp.empty())) {
                arg5_temp += c;
            } else if (!isspace(c)) {
                cerr << "syscall-lang: error: fifth argument must be integer!" << endl;
                exit(1);
            }
        } else if (!arg6_ready) {
            if (isdigit(c) || (c == '-' && arg6_temp.empty())) {
                arg6_temp += c;
            } else if (!isspace(c)) {
                cerr << "syscall-lang: error: sixth argument must be integer!" << endl;
                exit(1);
            }
        } else if (!arg7_ready) {
            if (isdigit(c) || (c == '-' && arg7_temp.empty())) {
                arg7_temp += c;
            } else if (!isspace(c)) {
                cerr << "syscall-lang: error: seventh argument must be integer!" << endl;
                exit(1);
            }
        }
    }
    
    if (!arg1_ready && !arg1_temp.empty()) {
        arg1 = stol(arg1_temp);
        arg1_ready = true;
        arg_count++;
    }
    if (!arg2_ready && !arg2_temp.empty()) {
        arg2 = stol(arg2_temp);
        arg2_ready = true;
        arg_count++;
    }
    if (!arg3_ready && (!arg3_temp.empty() || is_arg3_string)) {
        if (!is_arg3_string) arg3 = stol(arg3_temp);
        arg3_ready = true;
        arg_count++;
    }
    if (!arg4_ready && !arg4_temp.empty()) {
        arg4 = stol(arg4_temp);
        arg4_ready = true;
        arg_count++;
    }
    if (!arg5_ready && !arg5_temp.empty()) {
        arg5 = stol(arg5_temp);
        arg5_ready = true;
        arg_count++;
    }
    if (!arg6_ready && !arg6_temp.empty()) {
        arg6 = stol(arg6_temp);
        arg6_ready = true;
        arg_count++;
    }
    if (!arg7_ready && !arg7_temp.empty()) {
        arg7 = stol(arg7_temp);
        arg7_ready = true;
        arg_count++;
    }
    
    if (arg1_ready && arg_count >= 1) {
        long return_code;
        
        if (is_arg3_string) {
            string processed_str = "";
            for (size_t i = 0; i < arg3_string.length(); ++i) {
                if (arg3_string[i] == '\\' && i + 1 < arg3_string.length() && arg3_string[i+1] == 'n') {
                    processed_str += '\n';
                    i++;
                } else {
                    processed_str += arg3_string[i];
                }
            }
            
            if (arg_count == 1) {
                return_code = syscall(arg1);
            } else if (arg_count == 2) {
                return_code = syscall(arg1, arg2);
            } else if (arg_count == 3) {
                return_code = syscall(arg1, arg2, processed_str.c_str());
            } else if (arg_count == 4) {
                return_code = syscall(arg1, arg2, processed_str.c_str(), arg4);
            } else if (arg_count == 5) {
                return_code = syscall(arg1, arg2, processed_str.c_str(), arg4, arg5);
            } else if (arg_count == 6) {
                return_code = syscall(arg1, arg2, processed_str.c_str(), arg4, arg5, arg6);
            } else if (arg_count == 7) {
                return_code = syscall(arg1, arg2, processed_str.c_str(), arg4, arg5, arg6, arg7);
            }
        } else {
            if (arg_count == 1) {
                return_code = syscall(arg1);
            } else if (arg_count == 2) {
                return_code = syscall(arg1, arg2);
            } else if (arg_count == 3) {
                return_code = syscall(arg1, arg2, arg3);
            } else if (arg_count == 4) {
                return_code = syscall(arg1, arg2, arg3, arg4);
            } else if (arg_count == 5) {
                return_code = syscall(arg1, arg2, arg3, arg4, arg5);
            } else if (arg_count == 6) {
                return_code = syscall(arg1, arg2, arg3, arg4, arg5, arg6);
            } else if (arg_count == 7) {
                return_code = syscall(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
            }
        }
        
        if (return_code == -1) {
            cerr << "syscall-lang: error: " << line << " executed with error." << endl;
        }
    } else if (!line.empty() && line[0] != '#') {
        cerr << "syscall-lang: error: need at least syscall number and one argument" << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        string arg1 = argv[1];
        if (arg1 == "--help" || arg1 == "help") {
            std::cout << "MicroOS syscall-lang v0.3" << std::endl;
            std::cout << "Usage: syscall-lang [file]" << std::endl;
            std::cout << "A low-level language for direct Linux system calls execution." << std::endl;
            std::cout << "Syntax:" << std::endl;
            std::cout << "  [syscall_number] [arg1] [arg2/\"string\"] [arg3] [arg4] [arg5] [arg6]" << std::endl;
            std::cout << "  - Minimum 2 arguments, maximum 7 arguments." << std::endl;
            std::cout << "  - Arguments must be integers." << std::endl;
            std::cout << "  - The third argument can be a string enclosed in double quotes." << std::endl;
            std::cout << "  - Comments start with #." << std::endl;
            std::cout << "Example (Hello World):" << std::endl;
            std::cout << "  1 1 \"Hello, World!\\n\" 14" << std::endl;
            return 0;
        }
        ifstream file(arg1);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                execute_line(line);
            }
        } else {
            cerr << "syscall-lang: error while opening file " << arg1 << endl;
            return 1;
        }
    } else {
        cout << "MicroOS syscall-lang v0.3" << endl;
        cout << "Type exit to quit" << endl;
        while (true) {
            string line;
            cout << "syscall> ";
            if (!getline(cin, line) || line == "exit") {
                return 0;
            }
            if (!line.empty()) {
                execute_line(line);
            }
        }
    }
    return 0;
}