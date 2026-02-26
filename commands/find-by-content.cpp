#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        string path = argv[1];
        if (!fs::is_directory(path)) {
            cerr << "find-by-content: " << path << " is not a directory" << endl;
            return 1;
        }
        string content;
        for (int i = 2; i < argc; i++) {
            content += argv[i];
            if (i != argc-1) {
                content += " ";
            }
        }
        try {
            bool found = false;
            for (const auto& entry : fs::recursive_directory_iterator(path)) {
                try {
                    if (entry.is_regular_file()) {
                        ifstream file (entry.path());
                        if (file.is_open()) {
                            string line;
                            while (getline(file, line)) {
                                if (line.find(content) != string::npos) {
                                    cout << "Found " << content << " in " << entry.path() << endl;
                                    found = true;
                                    break;
                                }
                            }
                            file.close();
                        } else {
                            cerr << "find-by-content: error while reading " << entry.path() << " file" << endl;
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    cerr << "find-by-content: error while reading " << entry.path() << ": " << e.what() << endl;
                }
            }
            if (!found) {
                cerr << "find-in-file: " << content << " not found in " << argv[1] << endl;
                return 1;
            }
        } catch (const fs::filesystem_error& e) {
            cerr << "find-by-content: error while reading " << path << " directory: " << e.what() << endl;
            return 1;
        }
    } else {
        cerr << "Usage: find-by-content [directory] [content]" << endl;
        return 1;
    }
    return 0;
}