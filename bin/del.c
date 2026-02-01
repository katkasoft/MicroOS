#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: del [filename]\n");
        return 1;
    }

    if (unlink(argv[1]) == 0) {
        return 0;
    } else {
        fprintf(stderr, " del: error deleting file: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}