#include <iostream>
#include <dirent.h>
#include <ctype.h>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <signal.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        string processName = argv[1];
        int sig;
        if (argc == 3) {
            sig = atoi(argv[2]);
        } else {
            sig = 15;
        }
        bool found = false;
        DIR* dir = opendir("/proc");
        if (!dir) {
            cerr << "stop-by-name: error while trying to get list of processes" << endl;
        }
        try {
            struct dirent* entry;
            while ((entry = readdir(dir))) {
                if (isdigit(entry->d_name[0])) {
                    char* pid = entry->d_name;
                    string path = "/proc/" + string(pid) + "/comm";
                    ifstream processFile(path);
                    stringstream process;
                    process << processFile.rdbuf();
                    processFile.close();
                    if (processName == process.str()) {
                        found = true;
                        int intPid = stoi(pid);
                        if (kill(intPid, sig) == -1) {
                            cerr << "stop-by-name: error while killing process" << endl;
                            return 1;
                        }
                    }
                }
            }
            closedir(dir);
            if (!found) {
                cerr << "stop-by-name: process " << processName << " not found" << endl;
                return 1;
            }
        } catch (...) {
            cerr << "stop-by-name: error whule getting process list" << endl;
            return 1;
        }
    } else {
        cerr << "Usage: stop-by-name [process] [sig]" << endl;
        return 1;
    }
    return 0;
}
