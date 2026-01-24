#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::string path = ".";

    if (argc > 1) {
        if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "help") {
            std::cout << "MicroOS sd v0.1" << std::endl;
            std::cout << "This tool allows you to scan directory" << std::endl;
            std::cout << "Usage: sd [dir] [args], sd --help/help" << std::endl;
            std::cout << "Args: -f/--full-path - print full path of files" << std::endl;
            return 0;
        }
        path = argv[1];
    }

    try {
        if (!fs::exists(path)) {
            std::cerr << "sd: " << path << ": No such file or directory" << std::endl;
            return 1;
        }

        for (const auto & entry : fs::directory_iterator(path)) {
            if (argc > 2 && (std::string(argv[2]) == "-f" || std::string(argv[2]) == "--full-path")) {
                std::cout << entry.path().string() << std::endl;
            } else {
                std::cout << entry.path().filename().string() << "  ";
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