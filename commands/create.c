#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void create_file(char* filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr != NULL) {
        fclose(fptr);
    } else {
        printf("Error: could not create file %s\n", filename);
    }
}

void create_dir(char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0755);
    }
}

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        char* path = argv[2];
        if (strcmp(argv[1], "file") == 0) {
            create_file(path);
        } else if (strcmp(argv[1], "dir") == 0 || strcmp(argv[1], "directory") == 0) { 
            create_dir(path);
        } else {
            printf("No such option: %s\n", argv[1]);
        }
    } else {
        printf("Usage: create [file/dir] [name]\n");
    }
    return 0;
}