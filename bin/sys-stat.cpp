#include <iostream>
#include <sys/sysinfo.h>
#include <string>
#include <fstream>
#include <vector>
#include <numeric>
#include <thread>
#include <chrono>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>

using namespace std;

struct CPUData {
    unsigned long long totalTime;
    unsigned long long idleTime;
};

CPUData getCPUStats() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    
    std::string label;
    std::vector<unsigned long long> values;
    unsigned long long val;
    
    file.seekg(0);
    file >> label;
    while (file >> val) {
        values.push_back(val);
    }

    unsigned long long idle = values[3] + values[4];
    unsigned long long total = std::accumulate(values.begin(), values.end(), 0ULL);
    
    return {total, idle};
}

double calculateUsage(const CPUData& prev, const CPUData& curr) {
    double totalDelta = curr.totalTime - prev.totalTime;
    double idleDelta = curr.idleTime - prev.idleTime;
    return 100.0 * (1.0 - (idleDelta / totalDelta));
}

string get_cpu() {
    CPUData d1 = getCPUStats();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    CPUData d2 = getCPUStats();
    long long usage = (long long)calculateUsage(d1, d2); 
    return to_string(usage) + "%";
}

string get_disk() {
    struct statvfs stats;
    if (statvfs("/", &stats) != 0) {
        return "error";
    }
    unsigned long long totalMB = (stats.f_blocks * stats.f_frsize) / (1024 * 1024);
    unsigned long long freeMB = (stats.f_bavail * stats.f_frsize) / (1024 * 1024);
    unsigned long long usedMB = totalMB - freeMB;

    return std::to_string(usedMB) + "/" + std::to_string(totalMB) + " Mb";
}

string get_ram() {
    struct sysinfo memInfo;
    if (sysinfo(&memInfo) != 0) return "error";
    long long totalPhysMem = (long long)memInfo.totalram * memInfo.mem_unit;
    long long physMemUsed = (long long)(memInfo.totalram - memInfo.freeram) * memInfo.mem_unit;
    return to_string(physMemUsed / (1024 * 1024)) + "/" + to_string(totalPhysMem / (1024 * 1024)) + " Mb";
}

string get_uptime() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return to_string(info.uptime);
    }
    return "error";
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "--help" || string(argv[1]) == "help") {
            std::cout << "MicroOS sys-stat v0.2" << std::endl;
            std::cout << "Usage: sys-stat [args]" << std::endl;
            std::cout << "args: -r/--ram - show only ram usage" << std::endl;
            std::cout << "      -c/--cpu - show only cpu usage" << std::endl;
            std::cout << "      -d/--disk - show only disk usage" << std::endl;
            std::cout << "      -u/--uptime - show only uptime" << std::endl;
            return 0;
        } else if (string(argv[1]) == "-r" || string(argv[1]) == "--ram") {
            string ram_usage = get_ram();
            if (ram_usage == "error") {
                cerr << "sys-stat: rror while getting ram usage" << endl;
                return 1;
            } else {
                cout << ram_usage << endl;
                return 0;
            }
        } else if (string(argv[1]) == "-c" || string(argv[1]) == "--cpu") {
            string cpu_percentage = get_cpu();
            if (cpu_percentage == "%") {
                cerr << "sys-stat: error while getting cpu usage" << endl;
                return 1;
            } else {
                cout << cpu_percentage << endl;
                return 0;
            }
        } else if (string(argv[1]) == "-d" || string(argv[1]) == "--disk") {
            string disk_usage = get_disk();
            if (disk_usage == "error") {
                cerr << "sys-stat: error while getting disk usage" << endl;
                return 1;
            } else {
                cout << disk_usage << endl;
                return 0;
            }
        } else if (string(argv[1]) == "-u" || string(argv[1]) == "--uptime") {
            string uptime = get_uptime();
            if (uptime == "error") {
                cerr << "sys-stat: error uptime" << endl;
                return 1;
            } else {
                cout << uptime << "s" << endl;
                return 0;
            }
        }
    } else {
        string ram_usage = get_ram();
        if (ram_usage != "error") {
            cout << "RAM usage: " << ram_usage << endl;
        }
        string cpu_percentage = get_cpu();
        if (cpu_percentage != "%") {
            cout << "CPU usage: " << cpu_percentage << endl;
        }
        string disk_usage = get_disk();
        if (disk_usage != "error") {
            cout << "Disk (/) usage: " << disk_usage << endl;
        }
        string uptime = get_uptime();
        if (uptime != "error") {
            cout << "Uptime: " << uptime << "s" << endl;
        }
        return 0;
    }
}