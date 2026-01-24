#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mount.h>

int main() {
    printf("[INIT]: mounting /proc");
    mount("proc", "/proc", "proc", 0, NULL);
    printf("[INIT]: mounting /sys");
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    printf("[INIT]: mounting /dev");
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);

    while(1) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("[INIT]: initialising term");
            char *args[] = {"/bin/term", NULL};
            printf("\033[H\033[2J");
            execv("/bin/term", args);
            exit(1);
        } else {
            printf("[INIT]: term exited, powering off...");
            int status;
            wait(&status);
            char *args[] = {"/bin/poweroff", NULL};
            execv("/bin/poweroff", args);
        }
    }
    return 0;
}