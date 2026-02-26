#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        ifstream file (argv[1]);
        if (file.is_open()) {
            string content;
            for (int i = 2; i < argc; i++) {
                content += argv[i];
                if (i != argc-1) {
                    content += " ";
                }
            }
            string line;
            int count = 0;
            bool found = false;
            while (getline(file, line)) {
                count++;
                if (line.find(content) != string::npos) {
                    cout << "Found " << content << " in line " << count << ": " << line << endl;
                    found = true;
                }
            }
            file.close();
            if (!found) {
                cerr << "find-in-file: " << content << " not found in " << argv[1] << endl;
                return 1;
            }
        } else {
            cerr << "find-in-file: couldn't open file " << argv[1] << endl;
            return 1;
        }
    } else {
        cerr << "Usage: find-in-file [file] [content]" << endl;
        return 1;
    }
    return 0;
}