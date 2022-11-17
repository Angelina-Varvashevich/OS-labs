#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>

void killProcessById(DWORD id) {
    HANDLE hKillProc = OpenProcess(PROCESS_TERMINATE, FALSE, id);
    TerminateProcess(hKillProc, 1);
    CloseHandle(hKillProc);
}

void killProcessByName(const std::wstring& name) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry)) {
        std::wstring str(entry.szExeFile);
        if (str == name)
           killProcessById(entry.th32ProcessID);
    }
    while (Process32Next(snapshot, &entry)) {
        std::wstring str(entry.szExeFile);
        if (str == name)
            killProcessById(entry.th32ProcessID);
    }
    CloseHandle(snapshot);
}

void killByVariable(const std::wstring& variableName) {
    wchar_t szValue[200];
    DWORD lenValue = GetEnvironmentVariable(variableName.c_str(), szValue, sizeof(szValue) / sizeof(wchar_t));
    std::wstring names(szValue);
    int startCopyIndex = 0;
    for (int i = 0; i < lenValue; ++i) {
        if (szValue[i] == ',' || i==lenValue-1) {
            std::wstring nameToKill;
            if (i == lenValue - 1) {
                nameToKill = names.substr(startCopyIndex, i - startCopyIndex + 1);
            }
            else {
                nameToKill = names.substr(startCopyIndex, i - startCopyIndex);
            }
            startCopyIndex = i+1;
            killProcessByName(nameToKill);
        }
   }
}

int main(int argc, char* argv[])
{
    bool killByName = true;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--id") == 0) {
            killByName = false;
        }
        else if (strcmp(argv[i], "--name") == 0) {
            killByName = true;
        }
        else {
            if (killByName) {
                size_t cSize = strlen(argv[i]) + 1;
                wchar_t* wc = new wchar_t[cSize];
                size_t retValue;
                mbstowcs_s(&retValue, wc, cSize, argv[i], cSize);
                killProcessByName(wc);
                delete[] wc;
            }
            else {
                killProcessById(std::atoi(argv[i]));
            }
        }
    }
    std::wstring variableName = L"PROC_TO_KILL";
    killByVariable(variableName);
}
