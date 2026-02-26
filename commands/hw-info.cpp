#include <iostream>
#include <sys/sysinfo.h>
#include <string>
#include <fstream>
#include <sys/statvfs.h>
#include <unistd.h>
#include <limits.h>
#include <sys/statvfs.h>

using namespace std;

std::string get_cpu() {
    ifstream cpuFile("/proc/cpuinfo");
    if (!cpuFile.is_open()) return "error";
    string line;
    while (getline(cpuFile, line)) {
        if (line.find("model name") == 0) {
            size_t pos = line.find(": ");
            if (pos != string::npos)
                return line.substr(pos + 2);
        }
    }
    return "error";
}

string get_ram() {
    struct sysinfo memInfo;
    if (sysinfo(&memInfo) != 0) return "error";
    long long totalPhysMem = (long long)memInfo.totalram * memInfo.mem_unit;
    return to_string(totalPhysMem / (1024 * 1024)) + " Mb";
}

string get_disk() {
    struct statvfs stats;
    if (statvfs("/", &stats) != 0) {
        return "error";
    }
    unsigned long long totalMB = (stats.f_blocks * stats.f_frsize) / (1024 * 1024);
    return to_string(totalMB) + " Mb";
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "--help" || string(argv[1]) == "help") {
            std::cout << "MicroOS hw-info v0.1" << std::endl;
            std::cout << "Usage: hw-info [args]" << std::endl;
            std::cout << "args: -c/--cpu  - show only cpu model" << std::endl;
            std::cout << "      -r/--ram  - show only total ram" << std::endl;
            std::cout << "      -d/--disk - show only total disk capacity" << std::endl;
            return 0;
        } else if (string(argv[1]) == "-c" || string(argv[1]) == "--cpu") {
            string cpu = get_cpu();
            if (cpu == "error") {
                cerr << "hw-info: error while getting cpu info" << endl;
                return 1;
            } else {
                cout << cpu << endl;
                return 0;
            }
        } else if (string(argv[1]) == "-r" || string(argv[1]) == "--ram") {
            string ram = get_ram();
            if (ram == "error") {
                cerr << "hw-info: error while getting ram info" << endl;
                return 1;
            } else {
                cout << ram << endl;
                return 0;
            }
        } else if (string(argv[1]) == "-d" || string(argv[1]) == "--disk") {
            string disk = get_disk();
            if (disk == "error") {
                cerr << "hw-info: error while getting disk info" << endl;
                return 1;
            } else {
                cout << disk << endl;
                return 0;
            }
        }
    } else {
        string cpu = get_cpu();
        if (cpu != "error") {
            cout << "CPU Model: " << cpu << endl;
        }
        string ram = get_ram();
        if (ram != "error") {
            cout << "Total RAM: " << ram << endl;
        }
        string disk = get_disk();
        if (disk != "error") {
            cout << "Total Disk: " << disk << endl;
        }
        return 0;
    }
    return 0;
}
