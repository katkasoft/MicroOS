#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 3) {
        fs::path source = argv[1];
        fs::path destination = argv[2];
        try {
            fs::copy(source, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "copydir: error while copying dir: " << e.what() << std::endl;
        }
        return 0;
    } else {
        cout << "Usage: copydir [dir] [destitation]" << endl;
        return 1;
    }
}