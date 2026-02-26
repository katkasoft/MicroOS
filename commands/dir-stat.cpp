#include <iostream>
#include <filesystem>
#include <ostream>
#include <string>

namespace fs = std::filesystem;
using namespace std;

string get_size(const fs::path& path) {
    unsigned long long total_size = 0;
    try {
        auto options = fs::directory_options::skip_permission_denied;
        for (const auto& entry : fs::recursive_directory_iterator(path, options)) {
            if (fs::is_regular_file(entry.status())) {
                total_size += fs::file_size(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return "error";
    }
    if (total_size >= (1024*1024*1024)) {
        return to_string(total_size/(1024*1024*1024)) + " Gb";
    } else if (total_size >= (1024*1024)) {
        return to_string(total_size/(1024*1024)) + " Mb";
    } else if (total_size >= (1024)) {
        return to_string(total_size/(1024)) + " Kb";
    } else {
        return to_string(total_size) + " B";
    }
}

int get_permissions(const std::string& path) {
    try {
        auto status = fs::status(path);
        return static_cast<int>(status.permissions() & fs::perms::mask);
    } catch (const fs::filesystem_error& e) {
        return 1;
    }
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        string arg1 = argv[1];
        if (argc > 2) {
            string arg2 = argv[2];
            if (arg2 == "-s" || arg2 == "--size") {
                string size = get_size(arg1);
                if (size == "error") {
                    cerr << "dir-stat: error while getting directory size" << endl;
                    return 1;
                } else {
                    cout << size << endl;
                }
            } else if (arg2 == "-p"|| arg2 == "--permissions") {
                int permissions = get_permissions(arg1);
                if (permissions == 1) {
                    cerr << "dir-stat: error while getting directory permissions" << endl;
                    return 1;
                } else {
                    cout << permissions << endl;
                }
            }
        } else {
            if (arg1 == "--help" || arg1 == "help") {
                cout << "MicroOS dir-stat v0.1" << endl;
                cout << "Usage: dir-stat [dir] [args]" << endl;
                cout << "args: -s/--size - show only size" << endl;
                cout << "      -p/--permissions - show only permissions" << endl;
            } else {
                cout << "Directory: " << arg1 << endl;
                string size = get_size(arg1);
                if (size != "error") {
                    cout << "Size: " << size << endl;
                }
                int permissions = get_permissions(arg1);
                if (permissions != 1) {
                    cout << "Permissions: " << permissions << endl;
                }
            }
        }
    } else {
        cout << "Usage: dir-stat [dir] [args]" << endl;
        return 1;
    }
    return 0;
}
