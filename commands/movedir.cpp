#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 3) {
        fs::path source = argv[1];
        fs::path destination = argv[2];
        try {
            fs::rename(source, destination);
        } catch (const fs::filesystem_error& e) {
            try {
                fs::copy(source, destination, fs::copy_options::recursive);
                fs::remove_all(source);
            } catch (const fs::filesystem_error& e2) {
                std::cerr << "movedir: error while moving dir: " << e2.what() << std::endl;
                return 1;
            }
        }
        return 0;
    } else {
        cout << "Usage: movedir [dir] [destitation]" << endl;
        return 1;
    }
}