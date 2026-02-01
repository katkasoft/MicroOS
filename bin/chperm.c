#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 3) {
        char *path = argv[1];
        mode_t mode = strtol(argv[1], NULL, 8);
        if (chmod(path, mode) == 1) {
            perror("chper: error while changing permissions");
            return 1;
        }
        return 0;
    } else {
        printf("Usage: chper [file] [permissions]");
        return 1;
    }
}