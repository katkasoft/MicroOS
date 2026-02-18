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
#include <sstream>
#include <algorithm>
#include <sys/ioctl.h>

using namespace std;

struct CPUData {
    unsigned long long totalTime;
    unsigned long long idleTime;
};

struct ProcessInfo {
    string pid;
    string name;
    double mem_mb;
    string mem_str;
};

class SystemStats {
private:
    double get_mem_value(int pid) {
        string path = "/proc/" + to_string(pid) + "/statm";
        ifstream file(path);
        long rss_pages = 0;
        if (file >> rss_pages >> rss_pages) {
            long page_size = sysconf(_SC_PAGESIZE);
            return rss_pages * page_size;
        }
        return 0.0;
    }

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
        unsigned long long total = accumulate(values.begin(), values.end(), 0ULL);
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
        if (!dir) return;

        vector<ProcessInfo> procs;
        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (isdigit(entry->d_name[0])) {
                string pid_s = entry->d_name;
                ifstream f("/proc/" + pid_s + "/comm");
                string name;
                if (getline(f, name)) {
                    double m = get_mem_value(stoi(pid_s));
                    stringstream ss;
                    int mb = 1024.0*1024.0;
                    int kb = 1024.0;
                    if (m >= mb) {
                        ss << fixed << setprecision(1) << m/mb << " MB";
                    } else if (m >= kb) {
                        ss << fixed << setprecision(1) << m/kb << " KB";
                    } else {
                        ss << fixed << setprecision(1) << m << "  B";
                    }
                    procs.push_back({pid_s, name, m, ss.str()});
                }
            }
        }
        closedir(dir);

        sort(procs.begin(), procs.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.mem_mb > b.mem_mb;
        });

        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int max_rows = w.ws_row - 5;
        if (max_rows < 1) max_rows = 1;

        cout << left << setw(8) << "PID" << setw(12) << "Memory" << "Process" << endl;
        for (int i = 0; i < (int)procs.size() && i < max_rows; ++i) {
            cout << left << setw(8) << procs[i].pid << setw(12) << procs[i].mem_str << procs[i].name << endl;
        }
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
        if (read(STDIN_FILENO, &ch, 1) > 0) return (ch == 'q' || ch == 'Q');
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
        clearScreen();
    }
};

int main(int argc, char* argv[]) {
    if (argc == 2) {
        string arg = argv[1];
        if (arg == "--help" || arg == "help") {
            cout << "MicroOS monitor v0.3.1" << endl;
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
