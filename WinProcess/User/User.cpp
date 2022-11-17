#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include<string>

const TCHAR KILLER_PATH[] = L"..\\x64\\Debug\\Killer.exe";

struct process {
	const std::wstring NAME;
	const DWORD ID;
	process(std::wstring name, DWORD id) :  NAME(name), ID(id) {};
};

std::vector<process> getActiveProcesses() {
	std::vector<process> processes;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry)) {
		processes.push_back(process(std::wstring(entry.szExeFile), entry.th32ProcessID));
	}

	while (Process32Next(snapshot, &entry)) {
		processes.push_back(process(std::wstring(entry.szExeFile), entry.th32ProcessID));
	}
	CloseHandle(snapshot);
	return processes;
}

std::vector<std::wstring> getProcessNames(const std::wstring& variableNames, const std::vector<std::wstring>& argvKiller) {
	std::vector<std::wstring> names;
	int length = variableNames.size();
	int startCopyIndex = 0;
	for (int i = 0; i < length; ++i) {
		if (variableNames[i] == ',' || i == length - 1) {
			std::wstring nameToKill;
			if (i == length - 1) {
				nameToKill = variableNames.substr(startCopyIndex, i - startCopyIndex + 1);
				names.push_back(nameToKill);
			}
			else {
				nameToKill = variableNames.substr(startCopyIndex, i - startCopyIndex);
				names.push_back(nameToKill);
			}
			startCopyIndex = i + 1;
		}
	}
	bool procByName = true;
	length = argvKiller.size();
	for (int i = 0; i < length; i++) {
		if (argvKiller[i] == L"--name") {
			procByName = true;
		}
		else if (argvKiller[i] == L"--id") {
			procByName = false;
		}
		else if (procByName) {
			names.push_back(argvKiller[i]);
		}
	}
	return names;
}

std::vector<DWORD> getProcessIdies(const std::vector<std::wstring>& argvKiller) {
	std::vector<DWORD> idies;
	bool procByName = true;
	int length = argvKiller.size();
	for (int i = 0; i < length; i++) {
		if (argvKiller[i]== L"--id") {
			procByName = false;
		}
		else if (argvKiller[i] == L"--name") {
			procByName = true;
		}
		else {
			if (!procByName) {
				DWORD id = _wtol(argvKiller[i].c_str());
				idies.push_back(id);
			}
		}
	}
	return idies;
}

int main(){
	TCHAR szName[] = { L"Telegram.exe,Taskmgr.exe,notepad.exe" };
	SetEnvironmentVariable(L"PROC_TO_KILL", szName);

	TCHAR szInput[1000];
	std::wstring cmd;
	std::vector<std::wstring> szInputKiller = { L"--id", L"20460", L"--name", L"msedge.exe", L"Skype.exe", L"--id", L"21240" };
	for (int i = 0; i < szInputKiller.size(); ++i) {
		cmd += L" " + szInputKiller[i];
	}
	lstrcpy(szInput, cmd.c_str());
	
	std::vector<std::wstring> processNames = getProcessNames(szName, szInputKiller);
	std::vector<DWORD> processIdies = getProcessIdies(szInputKiller);
	std::vector<process> activeProcessesBeforeKill = getActiveProcesses();

	for (const process& currentpProcess : activeProcessesBeforeKill) {
		auto itId = std::find_if(processIdies.begin(), processIdies.end(), [&](DWORD id) { return id == currentpProcess.ID; });
		auto itName = std::find_if(processNames.begin(), processNames.end(), [&](const std::wstring& name) { return name == currentpProcess.NAME; });
		if (itId != processIdies.end() || itName != processNames.end()) {
			std::wcout << "Name: " << currentpProcess.NAME << " PID: " << currentpProcess.ID << " is active" << std::endl;
		}
	}

	STARTUPINFO         si = { sizeof(si) };
	SECURITY_ATTRIBUTES saProcess,
		saThread;
	PROCESS_INFORMATION piProcess;

	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = FALSE;

	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = FALSE;

	if (CreateProcess(KILLER_PATH, szInput, &saProcess, &saThread, FALSE, 0, NULL, NULL, &si, &piProcess)) {
		WaitForSingleObject(piProcess.hProcess, INFINITE);
		CloseHandle(piProcess.hThread);
		CloseHandle(piProcess.hProcess);
	}
	else {
		std::cerr << "Failed to create process." << std::endl;
		return 0;
	}  

	Sleep(3000);

	std::vector<process> activeProcessesAfterKill = getActiveProcesses();
	bool terminate = true;
	for (const process& currentpProcess : activeProcessesAfterKill) {
		auto itId = std::find_if(processIdies.begin(), processIdies.end(), [&](DWORD id) { return id == currentpProcess.ID; });
		auto itName = std::find_if(processNames.begin(), processNames.end(), [&](const std::wstring& name) { return name == currentpProcess.NAME; });
		if (itId != processIdies.end() || itName != processNames.end()) {
			terminate = false;
			std::cout << std::endl << std::endl;
			std::wcout << "Name: " << currentpProcess.NAME << " PID: " << currentpProcess.ID << " is still active" << std::endl;
		}
	}

	if (terminate) {
		std::cout << "All active processes were terminated";
	}
	else {
		std::cout << "Some processes still active";
	}
	SetEnvironmentVariable(L"PROC_TO_KILL", NULL);
}
