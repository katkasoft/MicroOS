#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 8192

int main(int argc, char* argv[]) {
    if (argc == 2) {
        char* filename = argv[1];
        FILE *fp = fopen("/tmp/buffer", "wb");
        FILE *fp2 = fopen("/tmp/copy_filename.txt", "w");

        if (!fp || !fp2) {
            perror("copy: error while creating temp files");
            if (fp) fclose(fp);
            if (fp2) fclose(fp2);
            return 1;
        }

        FILE *source_fp = fopen(filename, "rb");
        if (source_fp == NULL) {
            perror("copy: error opening source file");
            fclose(fp);
            fclose(fp2);
            return 1;
        }

        char buffer[BUFFER_SIZE];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), source_fp)) > 0) {
            fwrite(buffer, 1, bytes, fp);
        }

        fputs(filename, fp2);

        fclose(source_fp);
        fclose(fp);
        fclose(fp2);
        
        return 0;
    } else {
        fprintf(stderr, "Usage: copy [filename]\n");
        return 1;
    }
}