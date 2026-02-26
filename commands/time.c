#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (argc >= 2) {
        char buffer[80];
        char format[1024] = "";
        for (int i = 1; i < argc; i++) {
            strcat(format, argv[i]);
        }
        if (strftime(buffer, sizeof(buffer), format, timeinfo)) {
            printf("%s\n", buffer);
        } else {
            perror("time: error: invalid format");
        }
    } else {
        printf("%s\n", asctime(timeinfo));
    }
    return 0;
}