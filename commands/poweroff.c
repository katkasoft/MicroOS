#include <sys/reboot.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("Turning off computer... \n");
    sync();
    reboot(RB_POWER_OFF);
    return 0;
}