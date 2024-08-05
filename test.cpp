// #include "test.h"
#include <windows.h>
#include <iostream>

// 检测函数
void TestFunctions() {
    HWND hwnd = GetDesktopWindow();  // 获取桌面窗口句柄
    HDC hdc = GetDC(hwnd);           // 获取设备上下文句柄
    if (!hdc) {
        std::cout << "GetDC failed" << std::endl;
        return;
    } else {
        std::cout << "GetDC succeeded" << std::endl;
    }
    
    HDC hdcMem = CreateCompatibleDC(hdc);
    if (!hdcMem) {
        std::cout << "CreateCompatibleDC failed" << std::endl;
        ReleaseDC(hwnd, hdc);
        return;
    }

    RECT rc;
    GetClientRect(hwnd, &rc);

    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    if (!hBitmap) {
        std::cout << "CreateCompatibleBitmap failed" << std::endl;
        DeleteDC(hdcMem);
        ReleaseDC(hwnd, hdc);
        return;
    }

    SelectObject(hdcMem, hBitmap);

    // 调用PrintWindow
    if (!PrintWindow(hwnd, hdcMem, PW_CLIENTONLY)) {
        std::cout << "PrintWindow failed" << std::endl;
    } else {
        std::cout << "PrintWindow succeeded" << std::endl;
    }

    // 调用BitBlt
    if (!BitBlt(hdcMem, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdc, 0, 0, SRCCOPY)) {
        std::cout << "BitBlt failed" << std::endl;
    } else {
        std::cout << "BitBlt succeeded" << std::endl;
    }

    // 清理
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);
}


void test() {
    TestFunctions();
}

int main() {
    if (!RegisterHotKey(NULL, 1, 0, VK_DELETE)) {
		std::cout << "Failed to register hotkey" << std::endl;
		return 1;
	} else {
		std::cout << "Hotkey registered" << std::endl;
	}



    MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		if (msg.message == WM_HOTKEY) {
            test();
            }
        break;
    }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return 0;
}

