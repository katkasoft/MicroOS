#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
#include <unistd.h>

int main() {
    printf("[INIT]: mounting /proc\n");
    mount("proc", "/proc", "proc", 0, NULL);
    printf("[INIT]: mounting /sys\n");
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    printf("[INIT]: mounting /dev\n");
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);

    while(1) {
        pid_t pid = fork();
        if (pid == 0) {
            setsid();
            int fd = open("/dev/console", O_RDWR); 
            if (fd >= 0) {
                ioctl(fd, TIOCSCTTY, 1);
                dup2(fd, 0); dup2(fd, 1); dup2(fd, 2);
                if (fd > 2) close(fd);
            }

            char *args[] = {"/bin/term", NULL};
            execv("/bin/term", args);
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            printf("[INIT]: term exited, shutting down...\n");
            reboot(RB_POWER_OFF);
        }
    }
    return 0;
}