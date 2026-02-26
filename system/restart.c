#include <unistd.h>
#include <sys/reboot.h>
#include <stdio.h>

int main() {
    printf("Restarting computer... \n");
    sync();
    return reboot(RB_AUTOBOOT);
}
