#include <stdio.h>
  
int main(int argc, char* argv[])
{
    if (argc >= 2) {
        char buffer[256];
        char* filename = argv[1];
        FILE *fp = fopen(filename, "r");

        if(fp)
        {
            while((fgets(buffer, 256, fp)) != NULL)
            {
                printf("%s", buffer); 
            }
            fclose(fp);
            return 0;
        } else {
            printf("read: %s: No such file\n", filename);
            return 1;
        }
    } else {
        printf("Usage: read [file]\n");
        return 0;
    }
}