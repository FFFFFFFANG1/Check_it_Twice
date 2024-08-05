#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

void HookDLL(DWORD pid, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process:" << pid << std::endl;
        return;
    }
    LPVOID pRemoteBuffer = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteBuffer == NULL) {
        std::cerr << "Failed to allocate memory in remote process" << std::endl;
        return;
    }
    BOOL ret = WriteProcessMemory(hProcess, pRemoteBuffer, dllPath, strlen(dllPath) + 1, NULL);
    if (ret == 0) {
        std::cerr << "Failed to write to remote process" << std::endl;
        return;
    }
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pRemoteBuffer, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Failed to create remote thread" << std::endl;
        return;
    }
    WaitForSingleObject(hThread, INFINITE);
    DWORD exitCode;
    if (GetExitCodeThread(hThread, &exitCode) && exitCode != 0) {
        std::cout << "DLL injected successfully to" << pid << std::endl;
        return;
    } else {
        std::cerr << "Failed to inject DLL to" << pid << std::endl;
        return;
    }

    VirtualFreeEx(hProcess, pRemoteBuffer, strlen(dllPath)+1, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}



int main() {
    const char* dllPath = "C:\\Users\\86588\\source\\repos\\hbb_x86\\Debug\\hbb_x86.dll";
    std::vector<DWORD> pids;
    std::string input;
    std::cout << "Enter PID:" << std::endl;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string pid_str;
    while (std::getline(ss, pid_str, ',')) {
        DWORD pid = std::stoul(pid_str);
        pids.push_back(pid);
    }

    for (DWORD pid : pids) {
        std::cout << "Hooking PID: " << pid << std::endl;
        HookDLL(pid, dllPath);   
    }
    return 0;
}