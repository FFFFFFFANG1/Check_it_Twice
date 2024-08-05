#include "hbb.h"
#include <detours.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <psapi.h>
#include <tlhelp32.h>

// 初始化原始的BitBlt函数指针
pBitBlt oBitBlt = BitBlt;
pPrintWindow oPrintWindow = PrintWindow;

int screenShotCount = 0;

void Log(const std::string& message) {
    const std::string filepath = "C:\\Game_Screenshot\\log.txt";
    std::ofstream logfile;
    logfile.open(filepath, std::ios_base::app);
    if (logfile.is_open()) {
        logfile << message << std::endl;
    } else {
        std::cerr << "Failed to open log file." << std::endl;
    }
}

void PrintProcessInfo(int flag) {
    DWORD pid;
    pid = GetCurrentProcessId(); // 获取当前进程ID
    if (flag){
        Log("Screenshot Taken by PID: " + std::to_string(pid) + "using BitBlt");
    } else {
        Log("Screenshot Taken by PID: " + std::to_string(pid) + "using PrintWindow");
    }
}

void Save_Screenshot(HDC hdc, int width, int height) {
    HDC hdcMem = CreateCompatibleDC(hdc);
    if (!hdcMem) {
        std::cerr << "Failed to create compatible DC." << std::endl;
        return;
    }
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, width, height);
    if (!hbmMem) {
        std::cerr << "Failed to create compatible bitmap." << std::endl;
        DeleteDC(hdcMem);
        return;
    }
    SelectObject(hdcMem, hbmMem);
    
    if (!oBitBlt(hdcMem, 0, 0, width, height, hdc, 0, 0, SRCCOPY)){
        std::cerr << "Failed to BitBlt." << std::endl;
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        return;
    }
    
    BITMAPINFOHEADER biHeader;
    BITMAPFILEHEADER bfHeader;
    BITMAP bitmap;
    DWORD dwWritten;
    
    

    if (!GetObject(hbmMem, sizeof(BITMAP), &bitmap)){
        Log("Failed to get image because of GetObject().");
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        return;
    };

    bfHeader.bfType = 0x4D42; // "BM"
    bfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bitmap.bmWidthBytes * bitmap.bmHeight;
    bfHeader.bfReserved1 = 0;
    bfHeader.bfReserved2 = 0;
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = bitmap.bmWidth;
    biHeader.biHeight = bitmap.bmHeight;
    biHeader.biPlanes = 1;
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biSizeImage = bitmap.bmWidthBytes * bitmap.bmHeight;
    biHeader.biXPelsPerMeter = 0;
    biHeader.biYPelsPerMeter = 0;
    biHeader.biClrUsed = 0;
    biHeader.biClrImportant = 0;

    std::string filename = "C:\\Game_Screenshot\\screenshot" + std::to_string(screenShotCount) + ".bmp";
    std::wstring wfilename = std::wstring(filename.begin(), filename.end());
    HANDLE hFile = CreateFile(wfilename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create file." << std::endl;
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        return;
    }
    
    WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    BYTE* pBits = new BYTE[bitmap.bmWidthBytes * bitmap.bmHeight];
    if (!pBits) {
        std::cerr << "Failed to allocate memory." << std::endl;
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        CloseHandle(hFile);
        return;
    }
    GetDIBits(hdcMem, hbmMem, 0, bitmap.bmHeight, pBits, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS);
    WriteFile(hFile, pBits, bitmap.bmWidthBytes * bitmap.bmHeight, &dwWritten, NULL);

    CloseHandle(hFile);

    // 释放资源
    delete[] pBits;
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);

    screenShotCount += 1;
}


BOOL WINAPI hkBitBlt(HDC hdcDest, int xDest, int yDest, int width, int height, HDC hdcSrc, int xSrc, int ySrc, DWORD rop) {
    PrintProcessInfo(1);

    BOOL result = oBitBlt(hdcDest, xDest, yDest, width, height, hdcSrc, xSrc, ySrc, rop);

    if (result) {
        Save_Screenshot(hdcDest, width, height);
    }

    // 0 on fail
    return result;
}

BOOL WINAPI hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags) {
    PrintProcessInfo(0); 

    BOOL result = oPrintWindow(hwnd, hdcBlt, nFlags);

    std::cout << "result: " << result << std::endl;

    if (result) {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        Save_Screenshot(hdcBlt, width, height);
    }

    //0 on fail
    return result;
}



void Hook() {
    // start detours
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    // hook BitBlt
    LONG ret1 = DetourAttach(&(PVOID&)oBitBlt, hkBitBlt);
    LONG ret2 = DetourAttach(&(PVOID&)oPrintWindow, hkPrintWindow);
    // LONG ret3 = DetourAttach(&(PVOID&)oGetDC, hkGetDC);
    if (ret1 != NO_ERROR) {
        std::cerr << "Failed to hook BitBlt: " << ret1 << std::endl;
        return;
    }
    if (ret2 != NO_ERROR) {
        std::cerr << "Failed to hook PrintWindow: " << ret2 << std::endl;
        return;
    }

    // commit detours
    if (DetourTransactionCommit() == NO_ERROR) {
        std::cout << "Functions hooked successfully!" << std::endl;
    } else {
        std::cout << "Failed to hook." << std::endl;
    }
}

void Unhook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());


    DetourDetach(&(PVOID&)oBitBlt, hkBitBlt);
    DetourDetach(&(PVOID&)oPrintWindow, hkPrintWindow);

    DetourTransactionCommit();
}
