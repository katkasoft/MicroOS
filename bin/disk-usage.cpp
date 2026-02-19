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
            cerr << "disk-usage: error while getting mountpoints" << endl;
            return 1;
        }
        string mount_path;
        while ((mnt = getmntent(fp)) != nullptr) {
            if (device_name == mnt->mnt_fsname) {
                mount_path = mnt->mnt_dir;
                endmntent(fp);
                break;
            }
        }
        if (mount_path == "") {
            cerr << "disk-usage: error: " << device_name << " not mounted";
            return 1;
        }
        endmntent(fp);

        struct statvfs buf;
        if (statvfs(mount_path.c_str(), &buf) == 0) {            
            unsigned long long total_size = (unsigned long long)buf.f_frsize * buf.f_blocks;
            unsigned long long free_size = (unsigned long long)buf.f_frsize * buf.f_bavail;
            unsigned long long used_size = total_size - free_size;
            int gb = 1024 * 1024 * 1024;
            int mb = 1024 * 1024;
            int kb = 1024;
            if (used_size >= gb) {
                cout << used_size/gb << " GB / ";
            } else if (used_size >= mb) {
                cout << used_size/mb << " MB / ";
            } else if (used_size >= kb) {
                cout << used_size/kb << " KB / ";
            } else {
                cout << used_size << " B / ";
            }
            if (total_size >= gb) {
                cout << total_size/gb << " GB";
            } else if (total_size >= mb) {
                cout << total_size/mb << " MB";
            } else if (total_size >= kb) {
                cout << total_size/kb << " KB";
            } else {
                cout << total_size << " B";
            }
        } else {
            cerr << "disk-usage: error while getting disk usage" << endl;
        }
    } else {
        cout << "Usage: disk-usage [disk]" << endl;
        return 1;
    }
    return 0;
}