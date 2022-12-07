#include <unistd.h>
#include <sys/wait.h>
#include <vector>

void createProcess(int in, int out, const char* pathProcess){
    if (fork() == 0) {
        dup2(in, STDIN_FILENO);
        dup2(out, STDOUT_FILENO);//1
        close(in);
        close(out);
        execl(pathProcess, nullptr);
        exit(0);
    }
}

int main()//M-A-P-S
{
    const char *pathMProcess = "../../M/cmake-build-debug/M";
    const char *pathAProcess = "../../A/cmake-build-debug/A";
    const char *pathPProcess = "../../P/cmake-build-debug/P";
    const char *pathSProcess = "../../S/cmake-build-debug/S";

    int pipefdMA[2];
    int pipefdAP[2];
    int pipefdPS[2];
    int status;

    pipe(pipefdMA);
    pipe(pipefdAP);
    pipe(pipefdPS);

    int in = dup(STDIN_FILENO);
    int out = dup(STDOUT_FILENO);
    close(STDIN_FILENO);

    createProcess(in, pipefdMA[1], pathMProcess);
    createProcess(pipefdMA[0], pipefdAP[1], pathAProcess);
    createProcess(pipefdAP[0], pipefdPS[1], pathPProcess);
    createProcess(pipefdPS[0], out, pathSProcess);

    for (int i = 0; i < 4; ++i) {
        wait(&status);
    }
}