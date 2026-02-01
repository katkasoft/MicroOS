#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        char* filename = argv[1];
        fs::path path = ".";
        if (argc == 3) {
            path = argv[2];
        }
        try {
            auto options = fs::directory_options::skip_permission_denied;
            for (const auto& entry : fs::recursive_directory_iterator(path, options)) {
                if (entry.path().filename() == filename) {
                    std::cout << "find: " << fs::absolute(entry.path()) << std::endl;
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "find: error while founding file " << e.what() << std::endl;
            return 1;
        }
    } else {
        cout << "Usage: find [filename] [directory]" << endl;
        return 1;
    }
}