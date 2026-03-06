#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "../libraries/launch_command.hpp"

using namespace std;

void init_log(const string &message)
{
    cout << message << endl;
    ofstream log_file("/microos/log/init-log", ios::app);
    if (log_file.is_open())
    {
        log_file << message << endl;
        log_file.flush();
    }
}

int main()
{
    init_log("init: mounting /proc");
    if (mount("proc", "/proc", "proc", 0, NULL) < 0)
        init_log("init: error mounting /proc: " + string(strerror(errno)));
    init_log("init: mounting /sys");
    if (mount("sysfs", "/sys", "sysfs", 0, NULL) < 0)
        init_log("init: error mounting /sys: " + string(strerror(errno)));
    init_log("init: mounting /dev");
    if (mount("devtmpfs", "/dev", "devtmpfs", 0, NULL) < 0)
        init_log("init: error mounting /dev: " + string(strerror(errno)));
    init_log("init: launching services...");
    ifstream services_script("/microos/system/services.msh");
    if (services_script.is_open()) {
        string line;
        while (getline(services_script, line)) {
            init_log("init: launching " + line + " service...");
            int return_status = launch_command(line, false);
            if (return_status == 1) init_log("init: failed launching " + line + " service");
        }
        services_script.close();
    } else {
        init_log("init: error: couldn't open /sbin/services.msh script. services unable to load");
    }
    while (true) {
        init_log("init: launching start script...");
        ifstream start_script("/microos/system/start.msh");
        if (start_script.is_open()) {
            string line;
            while (getline(start_script, line)) {
                init_log("init: launching " + line);
                int return_status = launch_command(line);
                if (return_status == 1) init_log("init: failed launching " + line);
            }
            start_script.close();
        } else {
            init_log("init: error: couldn't open /sbin/start.msh script. system unable to load correctly");
            init_log("init: launching term...");
            int return_status = launch_command("term");
            if (return_status == 1) init_log("init: failed to launch term");
        }
        init_log("init: start exited. shutting down...");
        sync();
        reboot(RB_POWER_OFF);
        sleep(1);
    }
    return 0;
}