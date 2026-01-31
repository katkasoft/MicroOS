#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: del [filename] [args]\n");
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "help") == 0) {
        printf("MicroOS del v0.2\n");
        printf("This tool allows you to delete file or folder\n");
        printf("Usage: del [filename/dirname] [args], del --help/help\n"); 
        printf("Args: -d/--dir - delete dir (without - file)\n");
        return 0;
    }    

    if (argc >= 3 && (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "--dir") == 0)) {
        int rmdir_return = rmdir(argv[1]);
        if (rmdir_return == -1) {
            fprintf(stderr, "del: error deleting directory: %s\n", strerror(errno));
            return 1;
        }
        return 0;
    }

    if (unlink(argv[1]) == 0) {
        return 0;
    } else {
        fprintf(stderr, " del: error deleting file: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}