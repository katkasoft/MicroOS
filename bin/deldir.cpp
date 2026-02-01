#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 2) {
        fs::path dir = argv[1];
        try {
            fs::remove_all(dir);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "deldir: error while deleting dir " << e.what() << std::endl;
            return 1;
        }
        return 0;
    } else {
        cout << "Usage: deldir [dir]" << endl;
        return 1;
    }
}