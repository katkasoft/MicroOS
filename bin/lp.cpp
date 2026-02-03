#include <iostream>
#include <dirent.h>
#include <ctype.h>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

int main() {
    DIR* dir = opendir("/proc");
    if (!dir) {
        cerr << "lp: error while trying to get list of processes" << endl;
    }
    try {
        cout << "PID    Process" << endl;
        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (isdigit(entry->d_name[0])) {
                char* pid = entry->d_name;
                cout << pid;
                for (int i = 0; i<=(6-strlen(pid)); i++) {
                    cout << " ";
                }
                string path = "/proc/" + string(pid) + "/comm";
                ifstream processFile(path);
                string process;
                while (getline(processFile, process)) {
                    cout << process;
                }
                processFile.close();
                cout << endl;
            }
        }

        closedir(dir);
    } catch (...) {
        cerr << "lp: error whule getting process list" << endl;
        return 1;
    }
    
    return 0;
}
