#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

int launch_command(string command, bool wait = true) {
    stringstream ss(command);
    vector<string> tokens;
    string w;
    while (ss >> w) tokens.push_back(w);
    if (tokens.empty()) return -1;
    vector<char*> args;
    for (auto& s : tokens) {
        args.push_back(&s[0]);
    }
    args.push_back(nullptr);
    pid_t pid = fork();
    if (pid < 0) return 1;
    if (pid == 0) {
        if (!wait) {
            pid_t grandson = fork();
            if (grandson < 0) _exit(1);
            if (grandson > 0) _exit(0); 
            setsid(); 
            freopen("/dev/null", "r", stdin);
            freopen("/dev/null", "w", stdout);
            freopen("/dev/null", "w", stderr);
        }
        execvp(args[0], args.data());
        _exit(1);
        return 1;
    }
    if (wait) {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    } else {
        waitpid(pid, nullptr, 0); 
        return 0;
    }
}