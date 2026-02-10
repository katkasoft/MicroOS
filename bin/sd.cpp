#include <string>
#include <iostream>
#include <filesystem>
#include <sys/ioctl.h>
#include <unistd.h> 

using namespace std;
namespace fs = filesystem;

int main(int argc, char* argv[])
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_width = (w.ws_col > 0) ? w.ws_col : 80;
    string path = ".";

    if (argc > 1) {
        if (string(argv[1]) == "--help" || string(argv[1]) == "help") {
            cout << "MicroOS sd v0.2" << endl;
            cout << "Usage: sd [dir] [args]" << endl;
            cout << "args: -f/--full-path - show full path" << endl;
            cout << "      -r/--recursive - recursive scanning" << endl;
            return 0;
        }
        path = argv[1];
    }
    try {
        if (!fs::exists(path)) {
            cerr << "sd: " << path << ": No such directory" << endl;
            return 1;
        }
        if (argc > 2 && (string(argv[2]) == "-r" || string(argv[2]) == "--recursive")) {
            auto options = fs::directory_options::skip_permission_denied;
            for (const auto& entry : fs::recursive_directory_iterator(path, options)) {
                fs::path path = entry.path();
                if (fs::is_directory(path)) {
                    cout << entry.path().string() << "/" << endl;
                } else {
                    cout << entry.path().string() << endl;
                }
            }
        } else {
            int current_line_length = 0;
            for (const auto & entry : fs::directory_iterator(path)) {
                fs::path path = entry.path();
                if (argc > 2 && (string(argv[2]) == "-f" || string(argv[2]) == "--full-path")) {
                    if (fs::is_directory(path)) {
                        cout << path.string() << "/" << endl;
                    } else {
                        cout << path.string() << endl;
                    }
                } else {
                    string filename = path.filename().string();
                    int item_length = filename.length() + 2;
                    if (current_line_length + item_length > terminal_width) {
                        cout << "\n";
                        current_line_length = 0;
                    }
                    if (fs::is_directory(path)) {
                        cout << filename << "/ ";
                    } else {
                        cout << filename << "  ";
                    }
                    current_line_length += item_length;
                }
            }
            cout << endl;
        } 
    } 
    catch (const fs::filesystem_error& e) {
        cerr << "sd: error: " << e.what() << endl;
        return 1;
    }

    return 0;
}