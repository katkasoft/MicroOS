#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc == 3) {
        char* oldname = argv[1];
        char* newname = argv[2];
        if (rename(oldname, newname) == 0) {
            return 0;
        } else {
            perror("rename: renaming error");
            return 1;
        }
    } else {
        printf("Usage: rename [oldname] [newname]\n");
        return 1;
    }
}