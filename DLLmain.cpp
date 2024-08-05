
#include <Windows.h>
#include "hbb.h"
#include <iostream>

VOID __declspec(dllexport) havetohave(){
    std::cout << "test" << std::endl;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            std::cout << "DLL_PROCESS_ATTACH" << std::endl;
            Hook();
            break;
        case DLL_PROCESS_DETACH:
            Unhook();
            break;
    }
    return TRUE;
}