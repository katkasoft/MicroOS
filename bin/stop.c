#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        pid_t pid = atoi(argv[1]);
        int sig;
        if (argc == 3) {
            sig = atoi(argv[2]);
        } else {
            sig = 15;
        }
        if (kill(pid, sig) == -1) {
            perror("stop: error while killing process");
            return 1;
        }
        return 0;
    } else {
        printf("Usage: stop [pid] [sig]\n");
        return 1;
    }
}