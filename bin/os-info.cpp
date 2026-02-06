#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <sys/utsname.h>

using namespace std;

string microos_ver() {
    ifstream verFile("/etc/microos-ver.txt");
    if (!verFile.is_open()) return "error";
    stringstream buffer;
    buffer << verFile.rdbuf();
    return buffer.str();
}

string kernel_info() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        return "error";
    }
    return string(buffer.sysname) + " " + std::string(buffer.release);
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "--help" || string(argv[1]) == "help") {
            std::cout << "MicroOS os-info v0.1" << std::endl;
            std::cout << "Usage: os-info [args]" << std::endl;
            std::cout << "args: -v/--version - show only MicroOS version" << std::endl;
            std::cout << "      -k/--kernel - show only kernel info" << std::endl;
            return 0;
        } else if (string(argv[1]) == "-v" || string(argv[1]) == "--version") {
            string mo_ver = microos_ver();
            if (mo_ver == "error" || mo_ver == "") {
                cerr << "os-info: error while getting MicroOS version" << endl;
                return 1;
            } else {
                cout << mo_ver << endl;
                return 0;
            }
        } else if (string(argv[1]) == "-k" || string(argv[1]) == "--kernel") {
            string kernel = kernel_info();
            if (kernel == "error" || kernel == "") {
                cerr << "os-info: error while getting MicroOS version" << endl;
                return 1;
            } else {
                cout << kernel << endl;
                return 0;
            }
        }
    } else {
        string mo_ver = microos_ver();
        if (mo_ver != "error") {
            cout << "MicroOS version: " << mo_ver << endl;
        }
        string kernel = kernel_info();
        if (kernel != "error") {
            cout << "Kernel version: " << kernel << endl;
        }
        return 0;
    }
}