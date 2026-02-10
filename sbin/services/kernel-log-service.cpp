#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream in("/proc/kmsg");
    ofstream out;
    out.open("/var/log/kernel-log", ios::app);
    string line;
    if (in.is_open() && out.is_open()) {
        while (getline(in, line)) {
            out << line << endl;
        }
    } else {
        return 1;
    }
}