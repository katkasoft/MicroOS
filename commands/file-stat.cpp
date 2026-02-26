#include <iostream>
#include <filesystem>
#include <string>

using namespace std;
namespace fs = std::filesystem;

string get_permissions(const string& path) {
    try {
        auto p = fs::status(path).permissions();
        int numeric = static_cast<int>(p & fs::perms::mask);
        
        string first = to_string((numeric / 64) % 8);
        string second = to_string((numeric / 8) % 8);
        string third = to_string(numeric % 8);
        
        return first + second + third;
    } catch (const fs::filesystem_error& e) {
        cerr << "file-stat: Error while getting permissions: " << e.what() << endl;
        return "error";
    }
}

uintmax_t get_size(const string& path) {
    try {
        if (fs::exists(path) && fs::is_regular_file(path)) {
            return fs::file_size(path);
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "file-stat: error while getting file size: " << e.what() << endl;
    }
    return 0;
}

void print_size(uintmax_t size) {
    if (size >= 1024 * 1024 * 1024) {
        cout << size / (1024 * 1024 * 1024) << " GB";
    } else if (size >= 1024 * 1024) {
        cout << size / (1024 * 1024) << " MB";
    } else if (size >= 1024) {
        cout << size / 1024 << " KB";
    } else {
        cout << size << " B";
    }
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cerr << "Usage: file-stat [path] [args]" << endl;
        return 1;
    }

    string path = argv[1];

    if (!fs::exists(path)) {
        cerr << "file-stat: file not exists" << endl;
        return 1;
    }

    if (!fs::is_regular_file(path)) {
        cerr << "file-stat: " << path << " is not a regular file" << endl;
        return 1;
    }

    if (argc == 3) {
        string arg2 = argv[2];
        if (arg2 == "-p" || arg2 == "--permissions") {
            string permissions = get_permissions(path);
            if (permissions != "error") {
                cout << permissions << endl;
            }
            return 0;
        } else if (arg2 == "-s" || arg2 == "--size") {
            uintmax_t size = get_size(path);
            print_size(size);
            cout << endl;
            return 0;
        }
    } else if (string(argv[1]) == "--help" || string(argv[1]) == "help") {
        std::cout << "MicroOS file-stat v0.1" << std::endl;
        std::cout << "Usage: file-stat [path] [args]" << std::endl;
        std::cout << "args: -p/--permissions - show only permissions" << std::endl;
        std::cout << "      -s/--size - show only size" << std::endl;
        return 0;
    } else {
        cout << "File: " << path << endl;
        cout << "Size: ";
        print_size(get_size(path));
        cout << endl;
        cout << "Permissions: " << get_permissions(path) << endl;
    }

    return 0;
}