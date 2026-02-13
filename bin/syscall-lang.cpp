#include <cctype>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/syscall.h>
#include "../lib/syscalls.hpp"

using namespace std;

void execute_line(string line) {
    long arg1 = 0; string arg1_temp = ""; bool arg1_ready = false; bool is_arg1_key = false;
    long arg2 = 0; string arg2_temp = ""; bool arg2_ready = false; string arg2_string = ""; bool is_arg2_string = false;
    long arg3 = 0; string arg3_temp = ""; bool arg3_ready = false; string arg3_string = ""; bool is_arg3_string = false;
    long arg4 = 0; string arg4_temp = ""; bool arg4_ready = false; string arg4_string = ""; bool is_arg4_string = false;
    long arg5 = 0; string arg5_temp = ""; bool arg5_ready = false; string arg5_string = ""; bool is_arg5_string = false;
    long arg6 = 0; string arg6_temp = ""; bool arg6_ready = false; string arg6_string = ""; bool is_arg6_string = false;
    long arg7 = 0; string arg7_temp = ""; bool arg7_ready = false; string arg7_string = ""; bool is_arg7_string = false;
    bool quote_opened = false;
    int arg_count = 0;
    
    for (char c : line) {
        if (c == '#' && !quote_opened) {
            break;
        }
        if (c == ' ' && !quote_opened) {
            if (!arg1_ready && !arg1_temp.empty()) {
                if (is_arg1_key) {
                    auto it = syscalls.find(arg1_temp);
                    if (it != syscalls.end()) {
                        arg1 = it->second;
                    } else {
                        cerr << "syscall-lang: no such key: " << arg1_temp << endl;
                        exit(1);
                    }
                } else {
                    arg1 = stol(arg1_temp);
                }
                arg1_ready = true;
                arg_count++;
            } else if (arg1_ready && !arg2_ready && (!arg2_temp.empty() || is_arg2_string)) {
                if (!is_arg2_string) arg2 = stol(arg2_temp);
                arg2_ready = true;
                arg_count++;
            } else if (arg2_ready && !arg3_ready && (!arg3_temp.empty() || is_arg3_string)) {
                if (!is_arg3_string) arg3 = stol(arg3_temp);
                arg3_ready = true;
                arg_count++;
            } else if (arg3_ready && !arg4_ready && (!arg4_temp.empty() || is_arg4_string)) {
                if (!is_arg4_string) arg4 = stol(arg4_temp);
                arg4_ready = true;
                arg_count++;
            } else if (arg4_ready && !arg5_ready && (!arg5_temp.empty() || is_arg5_string)) {
                if (!is_arg5_string) arg5 = stol(arg5_temp);
                arg5_ready = true;
                arg_count++;
            } else if (arg5_ready && !arg6_ready && (!arg6_temp.empty() || is_arg6_string)) {
                if (!is_arg6_string) arg6 = stol(arg6_temp);
                arg6_ready = true;
                arg_count++;
            } else if (arg6_ready && !arg7_ready && (!arg7_temp.empty() || is_arg7_string)) {
                if (!is_arg7_string) arg7 = stol(arg7_temp);
                arg7_ready = true;
                arg_count++;
            }
            continue;
        }
        if (!arg1_ready) {
            if (arg1_temp.empty()) {
                arg1_temp += c;
                if (!isdigit(c)) {
                    is_arg1_key = true;
                }
            }
        } else if (!arg2_ready) {
            if (c == '"') {
                if (quote_opened) {
                    arg2_string = arg2_temp;
                    arg2_temp = "";
                    is_arg2_string = true;
                }
                quote_opened = !quote_opened;
                continue;
            }
            arg2_temp += c;
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
            if (c == '"') {
                if (quote_opened) {
                    arg4_string = arg4_temp;
                    arg4_temp = "";
                    is_arg4_string = true;
                }
                quote_opened = !quote_opened;
                continue;
            }
            arg4_temp += c;
        } else if (!arg5_ready) {
            if (c == '"') {
                if (quote_opened) {
                    arg5_string = arg5_temp;
                    arg5_temp = "";
                    is_arg5_string = true;
                }
                quote_opened = !quote_opened;
                continue;
            }
            arg5_temp += c;
        } else if (!arg6_ready) {
            if (c == '"') {
                if (quote_opened) {
                    arg6_string = arg6_temp;
                    arg6_temp = "";
                    is_arg6_string = true;
                }
                quote_opened = !quote_opened;
                continue;
            }
            arg6_temp += c;
        } else if (!arg7_ready) {
            if (c == '"') {
                if (quote_opened) {
                    arg7_string = arg7_temp;
                    arg7_temp = "";
                    is_arg7_string = true;
                }
                quote_opened = !quote_opened;
                continue;
            }
            arg7_temp += c;
        }
    }
    
    if (!arg1_ready && !arg1_temp.empty()) {
        arg1 = stol(arg1_temp);
        arg1_ready = true;
        arg_count++;
    }
    if (!arg2_ready && (!arg2_temp.empty() || is_arg2_string)) {
        if (!is_arg2_string) arg2 = stol(arg2_temp);
        arg2_ready = true;
        arg_count++;
    }
    if (!arg3_ready && (!arg3_temp.empty() || is_arg3_string)) {
        if (!is_arg3_string) arg3 = stol(arg3_temp);
        arg3_ready = true;
        arg_count++;
    }
    if (!arg4_ready && (!arg4_temp.empty() || is_arg4_string)) {
        if (!is_arg4_string) arg4 = stol(arg4_temp);
        arg4_ready = true;
        arg_count++;
    }
    if (!arg5_ready && (!arg5_temp.empty() || is_arg5_string)) {
        if (!is_arg5_string) arg5 = stol(arg5_temp);
        arg5_ready = true;
        arg_count++;
    }
    if (!arg6_ready && (!arg6_temp.empty() || is_arg6_string)) {
        if (!is_arg6_string) arg6 = stol(arg6_temp);
        arg6_ready = true;
        arg_count++;
    }
    if (!arg7_ready && (!arg7_temp.empty() || is_arg7_string)) {
        if (!is_arg7_string) arg7 = stol(arg7_temp);
        arg7_ready = true;
        arg_count++;
    }
    
    if (arg1_ready && arg_count >= 1) {
        long return_code;
        
        if (arg_count == 1) {
            return_code = syscall(arg1);
        } else if (arg_count == 2) {
            if (is_arg2_string) {
                string processed_str2 = "";
                for (size_t i = 0; i < arg2_string.length(); ++i) {
                    if (arg2_string[i] == '\\' && i + 1 < arg2_string.length() && arg2_string[i+1] == 'n') {
                        processed_str2 += '\n';
                        i++;
                    } else {
                        processed_str2 += arg2_string[i];
                    }
                }
                return_code = syscall(arg1, processed_str2.c_str());
            } else {
                return_code = syscall(arg1, arg2);
            }
        } else if (arg_count == 3) {
            if (is_arg2_string && is_arg3_string) {
                string processed_str2 = "";
                for (size_t i = 0; i < arg2_string.length(); ++i) {
                    if (arg2_string[i] == '\\' && i + 1 < arg2_string.length() && arg2_string[i+1] == 'n') {
                        processed_str2 += '\n';
                        i++;
                    } else {
                        processed_str2 += arg2_string[i];
                    }
                }
                string processed_str3 = "";
                for (size_t i = 0; i < arg3_string.length(); ++i) {
                    if (arg3_string[i] == '\\' && i + 1 < arg3_string.length() && arg3_string[i+1] == 'n') {
                        processed_str3 += '\n';
                        i++;
                    } else {
                        processed_str3 += arg3_string[i];
                    }
                }
                return_code = syscall(arg1, processed_str2.c_str(), processed_str3.c_str());
            } else if (is_arg2_string) {
                string processed_str2 = "";
                for (size_t i = 0; i < arg2_string.length(); ++i) {
                    if (arg2_string[i] == '\\' && i + 1 < arg2_string.length() && arg2_string[i+1] == 'n') {
                        processed_str2 += '\n';
                        i++;
                    } else {
                        processed_str2 += arg2_string[i];
                    }
                }
                return_code = syscall(arg1, processed_str2.c_str(), arg3);
            } else if (is_arg3_string) {
                string processed_str3 = "";
                for (size_t i = 0; i < arg3_string.length(); ++i) {
                    if (arg3_string[i] == '\\' && i + 1 < arg3_string.length() && arg3_string[i+1] == 'n') {
                        processed_str3 += '\n';
                        i++;
                    } else {
                        processed_str3 += arg3_string[i];
                    }
                }
                return_code = syscall(arg1, arg2, processed_str3.c_str());
            } else {
                return_code = syscall(arg1, arg2, arg3);
            }
        } else if (arg_count == 4) {
            if (is_arg2_string && is_arg3_string && is_arg4_string) {
                string processed_str2 = "", processed_str3 = "", processed_str4 = "";
                for (size_t i = 0; i < arg2_string.length(); ++i) {
                    if (arg2_string[i] == '\\' && i + 1 < arg2_string.length() && arg2_string[i+1] == 'n') { processed_str2 += '\n'; i++; } else { processed_str2 += arg2_string[i]; }
                }
                for (size_t i = 0; i < arg3_string.length(); ++i) {
                    if (arg3_string[i] == '\\' && i + 1 < arg3_string.length() && arg3_string[i+1] == 'n') { processed_str3 += '\n'; i++; } else { processed_str3 += arg3_string[i]; }
                }
                for (size_t i = 0; i < arg4_string.length(); ++i) {
                    if (arg4_string[i] == '\\' && i + 1 < arg4_string.length() && arg4_string[i+1] == 'n') { processed_str4 += '\n'; i++; } else { processed_str4 += arg4_string[i]; }
                }
                return_code = syscall(arg1, processed_str2.c_str(), processed_str3.c_str(), processed_str4.c_str());
            } else if (is_arg2_string && is_arg3_string) {
                string processed_str2 = "", processed_str3 = "";
                for (size_t i = 0; i < arg2_string.length(); ++i) {
                    if (arg2_string[i] == '\\' && i + 1 < arg2_string.length() && arg2_string[i+1] == 'n') { processed_str2 += '\n'; i++; } else { processed_str2 += arg2_string[i]; }
                }
                for (size_t i = 0; i < arg3_string.length(); ++i) {
                    if (arg3_string[i] == '\\' && i + 1 < arg3_string.length() && arg3_string[i+1] == 'n') { processed_str3 += '\n'; i++; } else { processed_str3 += arg3_string[i]; }
                }
                return_code = syscall(arg1, processed_str2.c_str(), processed_str3.c_str(), arg4);
            } else if (is_arg2_string && is_arg4_string) {
                string processed_str2 = "", processed_str4 = "";
                for (size_t i = 0; i < arg2_string.length(); ++i) {
                    if (arg2_string[i] == '\\' && i + 1 < arg2_string.length() && arg2_string[i+1] == 'n') { processed_str2 += '\n'; i++; } else { processed_str2 += arg2_string[i]; }
                }
                for (size_t i = 0; i < arg4_string.length(); ++i) {
                    if (arg4_string[i] == '\\' && i + 1 < arg4_string.length() && arg4_string[i+1] == 'n') { processed_str4 += '\n'; i++; } else { processed_str4 += arg4_string[i]; }
                }
                return_code = syscall(arg1, processed_str2.c_str(), arg3, processed_str4.c_str());
            } else if (is_arg3_string && is_arg4_string) {
                string processed_str3 = "", processed_str4 = "";
                for (size_t i = 0; i < arg3_string.length(); ++i) {
                    if (arg3_string[i] == '\\' && i + 1 < arg3_string.length() && arg3_string[i+1] == 'n') { processed_str3 += '\n'; i++; } else { processed_str3 += arg3_string[i]; }
                }
                for (size_t i = 0; i < arg4_string.length(); ++i) {
                    if (arg4_string[i] == '\\' && i + 1 < arg4_string.length() && arg4_string[i+1] == 'n') { processed_str4 += '\n'; i++; } else { processed_str4 += arg4_string[i]; }
                }
                return_code = syscall(arg1, arg2, processed_str3.c_str(), processed_str4.c_str());
            } else if (is_arg2_string) {
                string processed_str2 = "";
                for (size_t i = 0; i < arg2_string.length(); ++i) {
                    if (arg2_string[i] == '\\' && i + 1 < arg2_string.length() && arg2_string[i+1] == 'n') { processed_str2 += '\n'; i++; } else { processed_str2 += arg2_string[i]; }
                }
                return_code = syscall(arg1, processed_str2.c_str(), arg3, arg4);
            } else if (is_arg3_string) {
                string processed_str3 = "";
                for (size_t i = 0; i < arg3_string.length(); ++i) {
                    if (arg3_string[i] == '\\' && i + 1 < arg3_string.length() && arg3_string[i+1] == 'n') { processed_str3 += '\n'; i++; } else { processed_str3 += arg3_string[i]; }
                }
                return_code = syscall(arg1, arg2, processed_str3.c_str(), arg4);
            } else if (is_arg4_string) {
                string processed_str4 = "";
                for (size_t i = 0; i < arg4_string.length(); ++i) {
                    if (arg4_string[i] == '\\' && i + 1 < arg4_string.length() && arg4_string[i+1] == 'n') { processed_str4 += '\n'; i++; } else { processed_str4 += arg4_string[i]; }
                }
                return_code = syscall(arg1, arg2, arg3, processed_str4.c_str());
            } else {
                return_code = syscall(arg1, arg2, arg3, arg4);
            }
        } else if (arg_count == 5) {
            if (is_arg2_string && is_arg3_string && is_arg4_string && is_arg5_string) {
                string processed_str2, processed_str3, processed_str4, processed_str5;
                for (size_t i = 0; i < arg2_string.length(); ++i) { if (arg2_string[i] == '\\' && i+1 < arg2_string.length() && arg2_string[i+1] == 'n') { processed_str2 += '\n'; i++; } else { processed_str2 += arg2_string[i]; } }
                for (size_t i = 0; i < arg3_string.length(); ++i) { if (arg3_string[i] == '\\' && i+1 < arg3_string.length() && arg3_string[i+1] == 'n') { processed_str3 += '\n'; i++; } else { processed_str3 += arg3_string[i]; } }
                for (size_t i = 0; i < arg4_string.length(); ++i) { if (arg4_string[i] == '\\' && i+1 < arg4_string.length() && arg4_string[i+1] == 'n') { processed_str4 += '\n'; i++; } else { processed_str4 += arg4_string[i]; } }
                for (size_t i = 0; i < arg5_string.length(); ++i) { if (arg5_string[i] == '\\' && i+1 < arg5_string.length() && arg5_string[i+1] == 'n') { processed_str5 += '\n'; i++; } else { processed_str5 += arg5_string[i]; } }
                return_code = syscall(arg1, processed_str2.c_str(), processed_str3.c_str(), processed_str4.c_str(), processed_str5.c_str());
            } else {
                return_code = syscall(arg1, arg2, arg3, arg4, arg5);
            }
        } else if (arg_count == 6) {
            if (is_arg2_string && is_arg3_string && is_arg4_string && is_arg5_string && is_arg6_string) {
                string processed_str2, processed_str3, processed_str4, processed_str5, processed_str6;
                for (size_t i = 0; i < arg2_string.length(); ++i) { if (arg2_string[i] == '\\' && i+1 < arg2_string.length() && arg2_string[i+1] == 'n') { processed_str2 += '\n'; i++; } else { processed_str2 += arg2_string[i]; } }
                for (size_t i = 0; i < arg3_string.length(); ++i) { if (arg3_string[i] == '\\' && i+1 < arg3_string.length() && arg3_string[i+1] == 'n') { processed_str3 += '\n'; i++; } else { processed_str3 += arg3_string[i]; } }
                for (size_t i = 0; i < arg4_string.length(); ++i) { if (arg4_string[i] == '\\' && i+1 < arg4_string.length() && arg4_string[i+1] == 'n') { processed_str4 += '\n'; i++; } else { processed_str4 += arg4_string[i]; } }
                for (size_t i = 0; i < arg5_string.length(); ++i) { if (arg5_string[i] == '\\' && i+1 < arg5_string.length() && arg5_string[i+1] == 'n') { processed_str5 += '\n'; i++; } else { processed_str5 += arg5_string[i]; } }
                for (size_t i = 0; i < arg6_string.length(); ++i) { if (arg6_string[i] == '\\' && i+1 < arg6_string.length() && arg6_string[i+1] == 'n') { processed_str6 += '\n'; i++; } else { processed_str6 += arg6_string[i]; } }
                return_code = syscall(arg1, processed_str2.c_str(), processed_str3.c_str(), processed_str4.c_str(), processed_str5.c_str(), processed_str6.c_str());
            } else {
                return_code = syscall(arg1, arg2, arg3, arg4, arg5, arg6);
            }
        } else if (arg_count == 7) {
            if (is_arg2_string && is_arg3_string && is_arg4_string && is_arg5_string && is_arg6_string && is_arg7_string) {
                string processed_str2, processed_str3, processed_str4, processed_str5, processed_str6, processed_str7;
                for (size_t i = 0; i < arg2_string.length(); ++i) { if (arg2_string[i] == '\\' && i+1 < arg2_string.length() && arg2_string[i+1] == 'n') { processed_str2 += '\n'; i++; } else { processed_str2 += arg2_string[i]; } }
                for (size_t i = 0; i < arg3_string.length(); ++i) { if (arg3_string[i] == '\\' && i+1 < arg3_string.length() && arg3_string[i+1] == 'n') { processed_str3 += '\n'; i++; } else { processed_str3 += arg3_string[i]; } }
                for (size_t i = 0; i < arg4_string.length(); ++i) { if (arg4_string[i] == '\\' && i+1 < arg4_string.length() && arg4_string[i+1] == 'n') { processed_str4 += '\n'; i++; } else { processed_str4 += arg4_string[i]; } }
                for (size_t i = 0; i < arg5_string.length(); ++i) { if (arg5_string[i] == '\\' && i+1 < arg5_string.length() && arg5_string[i+1] == 'n') { processed_str5 += '\n'; i++; } else { processed_str5 += arg5_string[i]; } }
                for (size_t i = 0; i < arg6_string.length(); ++i) { if (arg6_string[i] == '\\' && i+1 < arg6_string.length() && arg6_string[i+1] == 'n') { processed_str6 += '\n'; i++; } else { processed_str6 += arg6_string[i]; } }
                for (size_t i = 0; i < arg7_string.length(); ++i) { if (arg7_string[i] == '\\' && i+1 < arg7_string.length() && arg7_string[i+1] == 'n') { processed_str7 += '\n'; i++; } else { processed_str7 += arg7_string[i]; } }
                return_code = syscall(arg1, processed_str2.c_str(), processed_str3.c_str(), processed_str4.c_str(), processed_str5.c_str(), processed_str6.c_str(), processed_str7.c_str());
            } else {
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
            std::cout << "MicroOS syscall-lang v0.5" << std::endl;
            std::cout << "Usage: syscall-lang [file]" << std::endl;
            std::cout << "A low-level language for direct Linux system calls execution." << std::endl;
            std::cout << "Syntax:" << std::endl;
            std::cout << "  [syscall key/syscall number] [arg1] [arg2/\"string\"] [arg3/\"string\"] [arg4/\"string\"] [arg5/\"string\"] [arg6/\"string\"]" << std::endl;
            std::cout << "  - Minimum 2 arguments, maximum 7 arguments." << std::endl;
            std::cout << "  - Arguments must be integers or double-quoted strings." << std::endl;
            std::cout << "  - The third argument can be a string enclosed in double quotes." << std::endl;
            std::cout << "  - Comments start with #." << std::endl;
            std::cout << "Example (Hello World):" << std::endl;
            std::cout << "  write 1 \"Hello, World!\\n\" 14" << std::endl;
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
        cout << "MicroOS syscall-lang v0.5" << endl;
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