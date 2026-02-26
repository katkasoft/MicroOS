#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

int main(int argc, char* argv[]) {
    if (argc == 3) {
        char* filename = argv[1];
        char* target = argv[2];
        char final_path[PATH_MAX];

        struct stat st;
        if (stat(target, &st) == 0 && S_ISDIR(st.st_mode)) {
            snprintf(final_path, sizeof(final_path), "%s/%s", target, filename);
        } else {
            strncpy(final_path, target, sizeof(final_path));
        }

        if (rename(filename, final_path) == 0) {
            return 0;
        } else {
            perror("moveto: error");
            return 1;
        }
    } else {
        fprintf(stderr, "Usage: moveto [file] [target_path]\n");
        return 1;
    }
}