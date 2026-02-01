#include <string>
#include <iostream>
#include <filesystem>
#include <sys/ioctl.h>
#include <unistd.h> 

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_width = (w.ws_col > 0) ? w.ws_col : 80;
    std::string path = ".";

    if (argc > 1) {
        if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "help") {
            std::cout << "MicroOS sd v0.2" << std::endl;
            std::cout << "Usage: sd [dir] [args]" << std::endl;
            std::cout << "args: -f/--full-path - show full path" << std::endl;
            std::cout << "      -r/--recursive - recursive scanning" << std::endl;
            return 0;
        }
        path = argv[1];
    }
    try {
        if (!fs::exists(path)) {
            std::cerr << "sd: " << path << ": No such directory" << std::endl;
            return 1;
        }
        if (argc > 2 && (std::string(argv[2]) == "-r" || std::string(argv[2]) == "--recursive")) {
            auto options = fs::directory_options::skip_permission_denied;
            for (const auto& entry : fs::recursive_directory_iterator(path, options)) {
                std::cout << entry.path().string() << std::endl;
            }
        } else {
            int current_line_length = 0;
            for (const auto & entry : fs::directory_iterator(path)) {
                if (argc > 2 && (std::string(argv[2]) == "-f" || std::string(argv[2]) == "--full-path")) {
                    std::cout << entry.path().string() << std::endl;
                } else {
                    std::string filename = entry.path().filename().string();
                    int item_length = filename.length() + 2;
                    if (current_line_length + item_length > terminal_width) {
                        std::cout << "\n";
                        current_line_length = 0;
                    }
                    std::cout << filename << "  ";
                    current_line_length += item_length;
                }
            }
            std::cout << std::endl;
        } 
    } 
    catch (const fs::filesystem_error& e) {
        std::cerr << "sd: error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}