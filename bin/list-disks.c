#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main() {
    const char* path = "/sys/block/";
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("list-disks: could not open /sys/block/ directory");
        return 1;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        if (strcmp(name, ".") == 0 || 
            strcmp(name, "..") == 0 || 
            strncmp(name, "loop", 4) == 0) {
            continue;
        }
        printf("/dev/%s\n", name);
    }
    closedir(dir);
    return 0;
}
