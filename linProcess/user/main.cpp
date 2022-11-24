#include <iostream>
#include <vector>
#include<string>
#include <unistd.h>
#include <wait.h>

const std::string KILLER_PATH = "/home/angelina/CLionProjects/linProcess/killer/cmake-build-debug/killer";

int main() {
    std::string name = "PROC_TO_KILL";
    std::string putVariableValue = {"Telegram,cheese,gnome-sudoku"};
    setenv(name.c_str(), putVariableValue.c_str(), 0);
    int pid;
    pid = fork();
    if (pid == 0) {
        char *szInputKiller[] = {"--id", "7878", "--id", "5481", "--name", "gnome-calculator", "--id", "5481",
                                 "--name", "gnome-calendar"};
        execv(KILLER_PATH.c_str(), szInputKiller);
        exit(0);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        unsetenv(name.c_str());
    } else {
        std::cerr << "Failed to fork" << std::endl;
    }
}
