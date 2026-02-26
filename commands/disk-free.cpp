#include <filesystem>
#include <iostream>
#include <mntent.h>
#include <string>
#include <sys/statvfs.h>
#include <cstdint>

namespace fs = std::filesystem;
using namespace std;

int main (int argc, char* argv[]) {
    if (argc == 2) {
        string device_name = argv[1];
        FILE* fp;
        struct mntent* mnt;
        fp = setmntent("/proc/self/mounts", "r");
        if (fp == nullptr) {
            cerr << "disk-free: error while getting mountpoints" << endl;
            return 1;
        }
        string mount_path;
        while ((mnt = getmntent(fp)) != nullptr) {
            if (device_name == mnt->mnt_fsname) {
                mount_path = mnt->mnt_dir;
                break;
            }
        }
        endmntent(fp);
        if (mount_path == "") {
            cerr << "disk-free: error: " << device_name << " not mounted" << endl;
            return 1;
        }

        struct statvfs buf;
        if (statvfs(mount_path.c_str(), &buf) == 0) {            
            unsigned long long total_size = (unsigned long long)buf.f_frsize * buf.f_blocks;
            unsigned long long free_size = (unsigned long long)buf.f_frsize * buf.f_bavail;
            int gb = 1024 * 1024 * 1024;
            int mb = 1024 * 1024;
            int kb = 1024;
            if (free_size >= gb) {
                cout << total_size/gb << " GB";
            } else if (free_size >= mb) {
                cout << free_size/mb << " MB";
            } else if (free_size >= kb) {
                cout << free_size/kb << " KB";
            } else {
                cout << free_size << " B" << endl;
            }
        } else {
            cerr << "disk-free: error while getting disk free space" << endl;
        }
    } else {
        cout << "Usage: disk-free [disk]" << endl;
        return 1;
    }
    return 0;
}