#include <string>
#include <iostream>
#include <filesystem>
#include <sys/ioctl.h> // ДЛЯ МАГИИ ШИРИНЫ (Linux/Unix)
#include <unistd.h>    // ДЛЯ STDOUT_FILENO

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_width = (w.ws_col > 0) ? w.ws_col : 80;
    std::string path = ".";

    if (argc > 1) {
        if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "help") {
            std::cout << "MicroOS sd v0.1" << std::endl;
            std::cout << "Usage: sd [dir] [-f]" << std::endl;
            return 0;
        }
        path = argv[1];
    }
    try {
        if (!fs::exists(path)) {
            std::cerr << "sd: " << path << ": No such directory" << std::endl;
            return 1;
        }
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
    catch (const fs::filesystem_error& e) {
        std::cerr << "sd: error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}