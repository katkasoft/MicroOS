#include <stdio.h>
  
int main()
{
    char buffer[512];
    char* filename = "/var/log/kernel-log";
    FILE *fp = fopen(filename, "r");
    if(fp) {
        while((fgets(buffer, 512, fp)) != NULL)
        {
            printf("%s", buffer); 
        }
        fclose(fp);
        return 0;
    } else {
        printf("kernel-log: error while opening kernel log in /var/log/kernel-log\n");
        return 1;
    }
}