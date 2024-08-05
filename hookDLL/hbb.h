#ifndef HBB_H
#define HBB_H

#include <windows.h>
#include <string>

// orig func ptr type
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


extern pBitBlt oBitBlt;
extern pPrintWindow oPrintWindow;
extern pGetDC oGetDC;

void Log(const std::string & message);

// helpers
void PrintProcessInfo();
void Save_Screenshot(HDC hdc, int width, int height);

// hooked Bitblt
BOOL WINAPI hkBitBlt(HDC hdcDest, int xDest, int yDest, int width, int height, HDC hdcSrc, int xSrc, int ySrc, DWORD rop);

// useless
BOOL WINAPI hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);


// hook
void Hook();
void Unhook();

#endif // HOOK_BITBLT_H
