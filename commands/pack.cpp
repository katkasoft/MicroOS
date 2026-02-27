#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 3) {
        string sourcePath = argv[1];
        string archivePath = argv[2];
        ofstream archive(archivePath, ios::app);
        if (!archive.is_open()) {
            cerr << "pack: couldn't open archive" << endl;
            return 1;
        }
        if (fs::exists(sourcePath) && fs::is_directory(sourcePath)) {
            for (const auto& entry : fs::recursive_directory_iterator(sourcePath)) {
                if (fs::is_regular_file(entry.path())) {
                    string fullPath = entry.path().string();
                    ifstream file(fullPath);
                    if (file.is_open()) {
                        archive << "!mpack " << fullPath << endl;
                        string line;
                        while (getline(file, line)) {
                            archive << line << endl;
                        }
                        file.close();
                    } else {
                        cerr << "pack: couldn't open file: " << fullPath << endl;
                        archive.close();
                        return 1;
                    }
                }
            }
        } else {
            ifstream file(sourcePath);
            if (file.is_open()) {
                archive << "!mpack " << sourcePath << endl;
                string line;
                while (getline(file, line)) {
                    archive << line << endl;
                }
                file.close();
            } else {
                cerr << "pack: couldn't open file: " << sourcePath << endl;
                archive.close();
                return 1;
            }
        }
        archive.close();
    } else {
        cout << "Usage: pack [file/dir] [archive]" << endl;
        return 1;
    }
    return 0;
}