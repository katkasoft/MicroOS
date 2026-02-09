#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("[INIT]: mounting /proc\n");
    if (mount("proc", "/proc", "proc", 0, NULL) < 0) 
        printf("[INIT]: error mounting /proc: %s\n", strerror(errno));

    printf("[INIT]: mounting /sys\n");
    if (mount("sysfs", "/sys", "sysfs", 0, NULL) < 0) 
        printf("[INIT]: error mounting /sys: %s\n", strerror(errno));

    printf("[INIT]: mounting /dev\n");
    if (mount("devtmpfs", "/dev", "devtmpfs", 0, NULL) < 0) 
        printf("[INIT]: error mounting /dev: %s\n", strerror(errno));
    
    while(1) {
        printf("[INIT]: forking for term...\n");
        pid_t pid = fork();

        if (pid < 0) {
            printf("[INIT]: fork failed: %s\n", strerror(errno));
            sleep(1);
            continue;
        }

        if (pid == 0) {
            printf("[INIT]: child process started, setting sid\n");
            setsid();

            printf("[INIT]: opening /dev/console\n");
            int fd = open("/dev/console", O_RDWR); 
            if (fd >= 0) {
                printf("[INIT]: console opened, setting controlling tty\n");
                ioctl(fd, TIOCSCTTY, 1);
                dup2(fd, 0); dup2(fd, 1); dup2(fd, 2);
                if (fd > 2) close(fd);
            } else {
                printf("[INIT]: failed to open /dev/console: %s\n", strerror(errno));
            }

            printf("[INIT]: executing /bin/term\n");
            char *args[] = {"/bin/term", NULL};
            execv("/bin/term", args);
            
            printf("[INIT]: execv /bin/term failed: %s\n", strerror(errno));
            exit(1);
        } else {
            int status;
            printf("[INIT]: waiting for term (PID: %d)...\n", pid);
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code == 2) {
                    printf("[INIT]: triggering kernel panic...");
                    return 1;
                }
                printf("[INIT]: term exited with status %d\n", exit_code);
            } else {
                printf("[INIT]: term terminated abnormally\n");
            }

            printf("[INIT]: shutting down...\n");
            reboot(RB_POWER_OFF);
        }
    }
    return 0;
}