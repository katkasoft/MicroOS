#include <iostream>
#include <dirent.h>
#include <ctype.h>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <numeric>
#include <thread>
#include <chrono>
#include <sys/sysinfo.h>
#include <iomanip>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

struct CPUData {
    unsigned long long totalTime;
    unsigned long long idleTime;
};

class SystemStats {
public:
    CPUData getCPUStats() {
        ifstream file("/proc/stat");
        string label;
        vector<unsigned long long> values;
        unsigned long long val;
        
        file >> label;
        while (file >> val) {
            values.push_back(val);
        }

        unsigned long long idle = values[3] + values[4];
        unsigned long long total = std::accumulate(values.begin(), values.end(), 0ULL);
        
        return {total, idle};
    }

    double calculateUsage(const CPUData& prev, const CPUData& curr) {
        double totalDelta = (double)curr.totalTime - prev.totalTime;
        double idleDelta = (double)curr.idleTime - prev.idleTime;
        if (totalDelta <= 0) return 0.0;
        return 100.0 * (1.0 - (idleDelta / totalDelta));
    }

    string getRAM() {
        struct sysinfo memInfo;
        if (sysinfo(&memInfo) != 0) return "error";
        long long totalPhysMem = (long long)memInfo.totalram * memInfo.mem_unit;
        long long physMemUsed = (long long)(memInfo.totalram - memInfo.freeram) * memInfo.mem_unit;
        return to_string(physMemUsed / (1024 * 1024)) + "/" + to_string(totalPhysMem / (1024 * 1024)) + " Mb";
    }

    void printProcessList() {
        DIR* dir = opendir("/proc");
        if (!dir) {
            cout << "Error reading /proc" << endl;
            return;
        }

        cout << left << setw(8) << "PID" << "Process" << endl;

        struct dirent* entry;
        int count = 0;
        while ((entry = readdir(dir)) && count < 20) {
            if (isdigit(entry->d_name[0])) {
                string pid = entry->d_name;
                string path = "/proc/" + pid + "/comm";
                ifstream processFile(path);
                string processName;
                if (getline(processFile, processName)) {
                    cout << left << setw(8) << pid << processName << endl;
                    count++;
                }
            }
        }
        closedir(dir);
    }
};

class KeyboardHandler {
public:
    static void setNonCanonicalMode(bool enable) {
        static struct termios oldt, newt;
        if (enable) {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            
            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        } else {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
        }
    }

    static bool checkQuit() {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0) {
            return (ch == 'q' || ch == 'Q');
        }
        return false;
    }
};

class Monitor {
private:
    SystemStats stats;
    CPUData lastCpuData;

    void clearScreen() {
        cout << "\033[2J\033[H";
    }
    
public:
    Monitor() {
        lastCpuData = stats.getCPUStats();
    }

    void run() {
        KeyboardHandler::setNonCanonicalMode(true);
        bool running = true;
        while (running) {
            clearScreen();
            CPUData currentCpuData = stats.getCPUStats();
            double cpuUsage = stats.calculateUsage(lastCpuData, currentCpuData);
            lastCpuData = currentCpuData;

            cout << "=== MicroOS System Monitor ===" << endl;
            cout << "CPU Usage: " << fixed << setprecision(1) << cpuUsage << "%" << endl;
            cout << "RAM Usage: " << stats.getRAM() << endl;
            cout << endl;

            stats.printProcessList();
            for (int i = 0; i < 10; ++i) {
                if (KeyboardHandler::checkQuit()) {
                    running = false;
                    break;
                }
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }
        KeyboardHandler::setNonCanonicalMode(false);
    }
};

int main(int argc, char* argv[]) {
    if (argc == 2) {
        string arg = argv[1];
        if (arg == "--help" || arg == "help") {
            cout << "MicroOS monitor v0.1" << endl;
            cout << "q to quit" << endl;
        } else {
            cerr << "monitor: no such argument: " << arg << endl;
            return 1;
        }
    } else {
        Monitor monitor;
        monitor.run();
    }
    return 0;
}