#include <fstream>
#include <iostream>
#include <ostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        string filename = argv[1];
        ofstream file;
        file.open(filename);
        if (file.is_open()) {
            for (int i = 2; i < argc; i++) {
                file << argv[i] << " ";
            }
            file << endl;
        } else {
            cerr << "writeto: error: could not open file: " << filename << endl;
            return 1;
        }
        file.close();
    } else {
        cout << "Usage: writeto [filename] [content]" << endl;
        return 1;
    }
    return 0;
}