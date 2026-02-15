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
#include <stdarg.h>

int log_fd = -1;

void init_log(const char *fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printf("%s", buffer);

    if (log_fd >= 0) {
        write(log_fd, buffer, strlen(buffer));
        fsync(log_fd);
    }
}

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
    
    log_fd = open("/var/log/init-log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        printf("[INIT]: error opening log file: %s\n", strerror(errno));
    }

    init_log("[INIT]: forking for services...\n");
    char* service_args[] = {(char*)"/sbin/term", (char*)"services", NULL};
    if (fork() == 0) {
        execv(service_args[0], service_args);
        _exit(1);
    }
    wait(NULL);

    while(1) {
        init_log("[INIT]: forking for start...\n");
        pid_t pid = fork();

        if (pid < 0) {
            init_log("[INIT]: fork failed: %s\n", strerror(errno));
            sleep(1);
            continue;
        }

        if (pid == 0) {
            init_log("[INIT]: child process started, setting sid\n");
            setsid();

            init_log("[INIT]: opening /dev/console\n");
            int fd = open("/dev/console", O_RDWR); 
            if (fd >= 0) {
                init_log("[INIT]: console opened, setting controlling tty\n");
                ioctl(fd, TIOCSCTTY, 1);
                dup2(fd, 0); dup2(fd, 1); dup2(fd, 2);
                if (fd > 2) close(fd);
            } else {
                init_log("[INIT]: failed to open /dev/console: %s\n", strerror(errno));
            }

            init_log("[INIT]: executing start\n");
            char *args[] = {"/sbin/term", "start", NULL};
            execv("/sbin/term", args);
            
            init_log("[INIT]: execv /sbin/term start failed: %s\n", strerror(errno));
            exit(1);
        } else {
            int status;
            init_log("[INIT]: waiting for start (PID: %d)...\n", pid);
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code == 2) {
                    init_log("[INIT]: triggering kernel panic...");
                    return 1;
                }
                init_log("[INIT]: start exited with status %d\n", exit_code);
            } else {
                init_log("[INIT]: start terminated abnormally\n");
            }

            init_log("[INIT]: shutting down...\n");
            sync();
            reboot(RB_POWER_OFF);
        }
    }
    return 0;
}