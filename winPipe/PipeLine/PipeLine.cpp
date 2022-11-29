#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>

int main()//M-A-P-S
{
	const wchar_t* pathMProcess = L"../x64/Debug/M.exe";
	const wchar_t* pathAProcess = L"../x64/Debug/A.exe";
	const wchar_t* pathPProcess = L"../x64/Debug/P.exe";
	const wchar_t* pathSProcess = L"../x64/Debug/S.exe";

	std::vector<HANDLE> handlePipes;

	auto closePipes = [&handlePipes]() {
		std::for_each(handlePipes.begin(), handlePipes.end(), CloseHandle);
	};

	SECURITY_ATTRIBUTES saPipe = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

	HANDLE hMAReadPipe, hMAWritePipe;
	bool MAPipe = CreatePipe(&hMAReadPipe, &hMAWritePipe, &saPipe, 0);
	if (!MAPipe) {
		std::cout << "MAPipe isn't created";
		return 1;
	}
	handlePipes.push_back(hMAReadPipe);
	handlePipes.push_back(hMAWritePipe);

	HANDLE hAPWritePipe, hAPReadPipe;
	bool APPipe = CreatePipe(&hAPReadPipe, &hAPWritePipe, &saPipe, 0);
	if (!APPipe) {
		std::cout << "APPipe isn't created";
		closePipes();
		return 1;
	}
	handlePipes.push_back(hAPReadPipe);
	handlePipes.push_back(hAPWritePipe);

	HANDLE hPSWritePipe, hPSReadPipe;
	bool PSPipe = CreatePipe(&hPSReadPipe, &hPSWritePipe, &saPipe, 0);
	if (!PSPipe) {
		std::cout << "PSPipe isn't create";
		closePipes();
		return 1;
	}
	handlePipes.push_back(hPSReadPipe);
	handlePipes.push_back(hPSWritePipe);

	STARTUPINFO startInfoProcessM, startInfoProcessA, startInfoProcessP, startInfoProcessS;
	
	GetStartupInfo(&startInfoProcessM);
	startInfoProcessM.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	startInfoProcessM.hStdOutput = hMAWritePipe;
	startInfoProcessM.dwFlags = STARTF_USESTDHANDLES;

	std::vector<PROCESS_INFORMATION> processes;
	auto closeAllProcesses = [&processes]() {
		std::for_each(processes.begin(), processes.end(), [](const PROCESS_INFORMATION& process_Info) {
			TerminateProcess(process_Info.hProcess, 1);
			CloseHandle(process_Info.hThread);
			CloseHandle(process_Info.hProcess);
			});
	};
	
	PROCESS_INFORMATION processM_Info;
	if (!CreateProcess(pathMProcess, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startInfoProcessM, &processM_Info)) {
		return 1;
	}
	processes.push_back(processM_Info);

	GetStartupInfo(&startInfoProcessA);
	startInfoProcessA.hStdInput = hMAReadPipe;
	startInfoProcessA.hStdOutput = hAPWritePipe;
	startInfoProcessA.dwFlags = STARTF_USESTDHANDLES;

	PROCESS_INFORMATION processA_Info;
	if (!CreateProcess(pathAProcess, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startInfoProcessA, &processA_Info)) {
		closeAllProcesses();
		return 1;
	}
	processes.push_back(processA_Info);

	GetStartupInfo(&startInfoProcessP);
	startInfoProcessP.hStdInput = hAPReadPipe;
	startInfoProcessP.hStdOutput = hPSWritePipe;
	startInfoProcessP.dwFlags = STARTF_USESTDHANDLES;

	PROCESS_INFORMATION processP_Info;
	if (!CreateProcess(pathPProcess, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startInfoProcessP, &processP_Info)) {
		closeAllProcesses();
		return 1;
	}
	processes.push_back(processP_Info);

	GetStartupInfo(&startInfoProcessS);
	startInfoProcessS.hStdInput = hPSReadPipe;
	startInfoProcessS.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	startInfoProcessS.dwFlags = STARTF_USESTDHANDLES;

	PROCESS_INFORMATION processS_Info;
	if (!CreateProcess(pathSProcess, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startInfoProcessS, &processS_Info)) {
		closeAllProcesses();
		return 1;
	}
	processes.push_back(processS_Info);

	std::for_each(processes.begin(), processes.end(), [](const PROCESS_INFORMATION& process_Info) {
		WaitForSingleObject(process_Info.hProcess, INFINITY);
		CloseHandle(process_Info.hThread);
		CloseHandle(process_Info.hProcess);
		});

	closePipes();
}


