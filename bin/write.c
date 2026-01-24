#include <stdio.h>
#include <string.h>

int main (int argc, char* argv[]) {
    if (argc >= 2) {
        char* filename = argv[1];
        FILE *fp = fopen(filename, "w");
        if (!fp) {
            printf("File not found or other error");
            return 1;
        }
        char line[256];
        printf("MicroOS write v0.1 Type 'q' to exit\n");
        while (fp) {
            if (fgets(line, sizeof(line), stdin) == NULL) break;
            line[strcspn(line, "\n")] = 0;
            if (strcmp(line, "q") == 0) {
                fclose(fp);
                return 0;
            }
            fputs(line, fp);
            fputs("\n", fp);

        }

    } else {
        printf("Usage: write [filename]\n");
    }
}