#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <unistd.h>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 2) {
        int fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            cerr << "disk-size: error while opening disk file" << endl; 
            return 1;
        }
        long long size = 0;
        if (ioctl(fd, BLKGETSIZE64, &size_bytes) == -1) {
            cerr << "disk-size: error getting disk size" << endl;
            close(fd);
            return 1;
        }
        close(fd);
    
        int gb = 1024.0 * 1024.0 * 1024.0;
        int mb = 1024.0 * 1024.0;
        int kb = 1024.0;
        if (size >= gb) {
            cout << size/gb << " GB" << endl;
        } else if (size >= mb) {
            cout << size/mb << " MB" << endl;
        } else if (size >= kb) {
            cout << size/kb << " KB" << endl;
        } else {
            cout << size << " B" << endl;
        }
        return 0;
    } else {
        cout << "Usage: disk-size [disk]" << endl;
        return 1;
    }
}