#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        char* path = "";
        if (argc == 2) {
            path = argv[1];
        }

        FILE* f = fopen("/tmp/copy_filename.txt", "r");
        if (!f) {
            printf("paste: error while reading file name\n");
            return 1;
        }

        fseek(f, 0, SEEK_END);
        long sz = ftell(f);
        fseek(f, 0, SEEK_SET);
        if (sz == 0) {
            printf("paste: nothing to paste\n");
            fclose(f);
            return 1;
        }
        char* buf = malloc(sz + 1);
        if (buf == NULL) {
            printf("paste: out of memory\n");
            fclose(f);
            return 1;
        }

        fread(buf, 1, sz, f);
        buf[sz] = 0;
        buf[strcspn(buf, "\r\n")] = 0; 
        char* filename = buf;
        fclose(f);

        FILE* f2 = fopen("/tmp/buffer", "r");
        if (!f2) {
            printf("paste: error while reading buffer\n");
            free(buf);
            return 1;
        }

        fseek(f2, 0, SEEK_END);
        long sz1 = ftell(f2);
        fseek(f2, 0, SEEK_SET);

        char* buf1 = malloc(sz1 + 1);
        if (buf1 == NULL) {
            printf("paste: out of memory\n");
            fclose(f2);
            free(buf);
            return 1;
        }

        fread(buf1, 1, sz1, f2);
        buf1[sz1] = 0;
        char* content = buf1;
        fclose(f2);

        char* final_path = filename;
        char* allocated_path = NULL;

        if (strcmp(path, "") != 0) {
            int total_length = strlen(path) + strlen(filename) + 2;
            allocated_path = malloc(total_length);
            if (allocated_path != NULL) {
                int has_slash = (path[strlen(path) - 1] == '/');
                snprintf(allocated_path, total_length, "%s%s%s", 
                        path, has_slash ? "" : "/", filename);
                final_path = allocated_path;
            }
        }

        FILE* fp = fopen(final_path, "w");
        if (!fp) {
            printf("paste: error while pasting\n");
            free(buf);
            free(buf1);
            free(allocated_path);
            return 1;
        }

        fwrite(content, 1, sz1, fp);
        fclose(fp);
        
        free(buf);
        free(buf1);
        free(allocated_path);
        return 0;
    } else {
        fprintf(stderr, "paste: path [filename]\n");
        return 1;
    }
}
