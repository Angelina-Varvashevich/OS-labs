#include <string>
#include <vector>
#include <iostream>

int killProcessById(long id) {
    std::string command = "kill -TERM " + std::to_string(id);
    return system(command.c_str());
}

int killProcessByName(const std::string &name) {
    std::string command = "killall -w " + name;
    return system(command.c_str());
}

void killByVariable(const std::string &variableName) {
    char *variableValue = getenv(variableName.c_str());
    if (variableValue == nullptr) {
        return;
    }
    std::string names(variableValue);
    int startCopyIndex = 0;
    int length = names.size();
    for (int i = 0; i < length; ++i) {
        if (names[i] == ',' || i == length - 1) {
            std::string nameToKill;
            if (i == length - 1) {
                nameToKill = names.substr(startCopyIndex, i - startCopyIndex + 1);
            } else {
                nameToKill = names.substr(startCopyIndex, i - startCopyIndex);
            }
            startCopyIndex = i + 1;
            int result = killProcessByName(nameToKill);
            std::cout << "Process " + nameToKill + " " << (result == 0 ? "was killed" : "can't be killed") << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    bool killByName = true;
    for (int i = 0; i < argc; i++) {
        std::string argvI = argv[i];
        if (argvI == "--id") {
            killByName = false;
        } else if (argvI == "--name") {
            killByName = true;
        } else {
            if (killByName) {
                int result = killProcessByName(argvI);
                std::cout << "Process " + argvI + " " << (result == 0 ? "was killed" : "can't be killed") << std::endl;
            } else {
                int result = killProcessById(std::stoi(argvI));
                std::cout << "Process with id " + argvI + " " << (result == 0 ? "was killed" : "can't be killed")
                          << std::endl;
            }
        }
    }
    killByVariable("PROC_TO_KILL");
}

