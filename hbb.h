#ifndef HBB_H
#define HBB_H

#include <windows.h>
#include <string>

// 声明原始的BitBlt函数类型
typedef BOOL(WINAPI* pBitBlt)(
    HDC hdcDest,
    int nXDest,
    int nYDest,
    int nWidth,
    int nHeight,
    HDC hdcSrc,
    int nXSrc,
    int nYSrc,
    DWORD dwRop
);

typedef BOOL(WINAPI* pPrintWindow)(
    HWND hwnd,
    HDC hdcBlt,
    UINT nFlags
);

typedef HDC(WINAPI* pGetDC)(
    HWND hWnd
);

// 声明原始的BitBlt函数指针
extern pBitBlt oBitBlt;
extern pPrintWindow oPrintWindow;
extern pGetDC oGetDC;

// 日志记录函数声明
void Log(const std::string & message);

// 打印进程信息函数声明
void PrintProcessInfo();
void Save_Screenshot(HDC hdc, int width, int height);

// hook的BitBlt函数声明
BOOL WINAPI hkBitBlt(HDC hdcDest, int xDest, int yDest, int width, int height, HDC hdcSrc, int xSrc, int ySrc, DWORD rop);
BOOL WINAPI hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);


// 函数用于挂钩和取消挂钩BitBlt
void Hook();
void Unhook();

#endif // HOOK_BITBLT_H
