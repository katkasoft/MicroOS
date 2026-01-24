#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main (int argc, char* argv[]) {
    if (argc >= 2) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "help") == 0) {
            printf("MicroOS write v0.2\n");
            printf("This tool allows you to write in file\n");
            printf("Usage: write [filename] [args], write --help/help\n");
            printf("Args: -a/--append - append lines to file (without - rewrite)\n");
            return 0;
        }
        char* filename = argv[1];
        bool append = ((argc > 2 && strcmp(argv[2], "-a") == 0) || (argc > 2 && strcmp(argv[2], "--append") == 0));
        FILE *fp;
        if (append) {
            fp = fopen(filename, "a");
        } else {
            fp = fopen(filename, "w");
        }
        if (!fp) {
            printf("File not found or other error");
            return 1;
        }
        char line[256];
        printf("MicroOS write v0.2 Type 'q' to exit\n");
        if (append) {
            char buffer[256];
            FILE *fpr = fopen(filename, "r");
            if (fpr) {
                while((fgets(buffer, 256, fpr)) != NULL)
                {
                    printf("%s", buffer); 
                }
                fclose(fpr);
            }
        }
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
        printf("Usage: write [filename] [args]\n");
    }
    return 0;
}