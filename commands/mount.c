#include <stdio.h>
#include <sys/mount.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc == 3) {
        const char *source = argv[1];
        const char *target = argv[2];
        const char *filesystem = "ext4";
        unsigned long flags = 0;
        const char *data = NULL;

        if (mount(source, target, filesystem, flags, data) == -1) {
            perror("mount: error while mounting");
            return 1;
        }
        return 0;
    } else {
        printf("Usage: mount [disk] [path]\n");
        return 1;
    }
}