#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc == 2) {
        char* filename = argv[1];
        FILE *fp = fopen("/etc/buffer", "w");
        FILE *fp2 = fopen("/etc/copy_filename.txt", "w");
        if (!fp || !fp2) {
            perror("Error while copying file");
            return 1;
        } else {
            int ch;
            FILE *source_fp = fopen(filename, "r");
            if (source_fp == NULL) {
                perror("Error opening source file");
                fclose(fp); fclose(fp2);
                return 1;
            }
            while ((ch = fgetc(source_fp)) != EOF) {
                fputc(ch, fp);
            }
            fputs(filename, fp2);

            fclose(source_fp);
            fclose(fp);
            fclose(fp2);
            return 0;
        }
    } else {
        printf("Usage: copy [filename]\n");
        return 1;
    }
}