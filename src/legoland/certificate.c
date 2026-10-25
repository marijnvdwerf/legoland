#include "legoland.h"

#define _WINSPOOL_
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "certificate.h"
#include "globals.h"
#include "imports.h"

// Print-job name.
// GLOBAL: LEGOLAND 0x004b86e8
static const char CertificateDocName[] = "Lego certificate";

// Prints the end-of-game certificate: bitmap_path (EGC.bmp) stretched over a landscape page,
// player_name centred on the certificate's name line and date_text in the top-left corner.
// Returns 1 if the page was printed, 0 if any step failed (each failure frees what it made).
//  1. Take the FIRST local printer from EnumPrintersA (not necessarily the default; no dialog).
//  2. Create a printer DC in landscape orientation.
//  3. Read the BMP by hand (file header, info header, palette for <= 8 bpp, pixels) and build
//     a device bitmap with CreateDIBitmap.
//  4. Give up if the printer cannot print bitmaps (RASTERCAPS & RC_BITBLT).
//  5. StartDoc("Lego certificate") / StartPage.
//  6. StretchDIBits the image over the page, with margins of 1/8 of the width left and right
//     and 1/64 of the height top and bottom.
//  7. Print the name with the 20 pt "Lego" font, centred, at page height * 678 / image height
//     (row 678 of the bitmap is the certificate's name line).
//  8. Print the date with the 8 pt light "Lego" font, left-aligned at (0, 0).
//  9. EndPage / EndDoc and free everything.
// Quirks kept from the original: the first font is never deleted (only the second), the
// memory DC and the bitmap selected into it are never drawn with, and the LocalAlloc'd
// LOGFONT is not checked for NULL.
// FUNCTION: LEGOLAND 0x00451740
int PrintCertificate(char *bitmap_path, char *player_name, char *date_text) {
    DEVMODEA dm;
    BYTE printers[0xa80];
    DOCINFOA di;
    BITMAPFILEHEADER bmfh;
    HDC hMemDC;
    BITMAPINFOHEADER bmih;
    DWORD needed;
    void *pBits;
    DWORD returned;
    BITMAPINFO *pInfo;
    HGLOBAL hInfo;
    HGLOBAL hBits;
    HBITMAP hBitmap;
    HFONT hFont;
    HFONT hOldFont;
    LOGFONTA *plf;
    int hres;
    HDC hDC;
    int vres;
    int fd;
    /* volatile: both only steer MSVC6's register choice (zero in ebp, fd in ebx); see docs/attempts */
    volatile int ncolors;
    volatile LONG x_pels_per_meter;
    needed = 0;
    pInfo = NULL;
    returned = 0;
    if (EnumPrintersA(1, NULL, 2, printers, 0x540, &needed, &returned) <= 0)
        return 0;
    if (returned <= 0)
        return 0;
    memset(&dm, 0, 0x94);
    dm.dmSize = 0x94;
    dm.dmFields = DM_ORIENTATION;
    dm.dmOrientation = DMORIENT_LANDSCAPE;
    hDC = CreateDCA(NULL, ((char **)printers)[1], NULL, &dm);
    if (!hDC)
        return 0;
    fd = _open(bitmap_path, _O_RDONLY | _O_BINARY, _S_IREAD);
    if ((int)fd >= 0) {
        _read(fd, &bmfh, 14);
        _read(fd, &bmih, 40);
        if (8 < bmih.biBitCount)
            ncolors = 0;
        else
            ncolors = 1 << bmih.biBitCount;
        hInfo = GlobalAlloc(GHND, ncolors * 4 + 40);
        if (hInfo != NULL) {
            pInfo = (BITMAPINFO *)GlobalLock(hInfo);
            if (NULL == pInfo) {
                GlobalFree(hInfo);
            } else {
                x_pels_per_meter = bmih.biXPelsPerMeter;
                pInfo->bmiHeader.biSize = bmih.biSize;
                pInfo->bmiHeader.biWidth = bmih.biWidth;
                pInfo->bmiHeader.biHeight = bmih.biHeight;
                pInfo->bmiHeader.biPlanes = bmih.biPlanes;
                pInfo->bmiHeader.biBitCount = bmih.biBitCount;
                pInfo->bmiHeader.biCompression = bmih.biCompression;
                pInfo->bmiHeader.biSizeImage = bmih.biSizeImage;
                pInfo->bmiHeader.biXPelsPerMeter = x_pels_per_meter;
                pInfo->bmiHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
                pInfo->bmiHeader.biClrUsed = bmih.biClrUsed;
                pInfo->bmiHeader.biClrImportant = bmih.biClrImportant;
                if (9 > pInfo->bmiHeader.biBitCount)
                    _read(fd, pInfo->bmiColors, (1 << bmih.biBitCount) * 4);
                hBits = GlobalAlloc(GHND, bmfh.bfSize - bmfh.bfOffBits);
                if (hBits == NULL) {
                    GlobalUnlock(hInfo);
                    GlobalFree(hInfo);
                    _close(fd);
                    DeleteDC(hDC);
                    return 0;
                }
                pBits = GlobalLock(hBits);
                if ((UINT_PTR)pBits == (INT_PTR)NULL) {
                    GlobalUnlock(hInfo);
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    _close(fd);
                    DeleteDC(hDC);
                    return 0;
                }
                _read(fd, pBits, bmfh.bfSize - bmfh.bfOffBits);
                hBitmap = CreateDIBitmap(hDC, &bmih, CBM_INIT, pBits, pInfo, 0);
                if (hBitmap == NULL) {
                    GlobalUnlock(hInfo);
                    GlobalUnlock(hBits);
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(NULL);
                    DeleteDC(hDC);
                    return 0;
                }
                GlobalUnlock(hInfo);
                GlobalUnlock(hBits);
                _close(fd);
                if (!(GetDeviceCaps(hDC, RASTERCAPS) & RC_BITBLT)) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    DeleteDC(hDC);
                    return 0;
                }
                di.cbSize = sizeof(DOCINFOA);
                di.lpszDocName = CertificateDocName;
                di.lpszOutput = NULL;
                di.lpszDatatype = NULL;
                di.fwType = 0;
                if (StartDocA(hDC, &di) == -1) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    DeleteDC(hDC);
                    return 0;
                }
                if (StartPage(hDC) <= 0) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    EndDoc(hDC);
                    DeleteDC(hDC);
                    return 0;
                }
                hMemDC = CreateCompatibleDC(hDC);
                if (hMemDC == NULL) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    EndPage(hDC);
                    EndDoc(hDC);
                    DeleteDC(hDC);
                    return 0;
                }
                if (SelectObject(hMemDC, hBitmap) == NULL) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    EndPage(hDC);
                    EndDoc(hDC);
                    DeleteDC(hDC);
                    return 0;
                }
                hres = GetDeviceCaps(hDC, HORZRES);
                vres = GetDeviceCaps(hDC, VERTRES);
                if (!StretchDIBits(hDC, hres / 8, vres / 64, hres - (hres / 8) * 2, vres - (vres / 64) * 2, 0, 0, bmih.biWidth, bmih.biHeight, pBits, pInfo, DIB_RGB_COLORS, SRCCOPY)) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    EndPage(hDC);
                    EndDoc(hDC);
                    DeleteDC(hDC);
                    return 0;
                }
                plf = (LOGFONTA *)LocalAlloc(LPTR, sizeof(LOGFONTA));
                plf->lfHeight = -MulDiv(20, GetDeviceCaps(hDC, LOGPIXELSY), 72);
                plf->lfWeight = 400;
                lstrcpyA(plf->lfFaceName, LegoFontName);
                hFont = CreateFontIndirectA(plf);
                if (!hFont) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    DeleteDC(hDC);
                    LocalFree(plf);
                    return 0;
                }
                SetBkMode(hDC, TRANSPARENT);
                hOldFont = (HFONT)SelectObject(hDC, hFont);
                SetBkMode(hDC, TRANSPARENT);
                SetTextAlign(hDC, TA_CENTER | TA_TOP);
                TextOutA(hDC, hres / 2, vres * 678 / pInfo->bmiHeader.biHeight, player_name, strlen(player_name));
                plf->lfHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);
                plf->lfWeight = 300;
                lstrcpyA(plf->lfFaceName, LegoFontName);
                hFont = CreateFontIndirectA(plf);
                if (NULL == hFont) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    DeleteDC(hDC);
                    LocalFree(plf);
                    return 0;
                }
                SetBkMode(hDC, TRANSPARENT);
                SelectObject(hDC, hFont);
                SetBkMode(hDC, TRANSPARENT);
                SetTextAlign(hDC, TA_LEFT);
                TextOutA(hDC, 0, 0, date_text, strlen(date_text));
                LocalFree(plf);
                SelectObject(hDC, hOldFont);
                DeleteObject(hFont);
                DeleteDC(hMemDC);
                if (EndPage(hDC) <= 0) {
                    GlobalFree(hInfo);
                    GlobalFree(hBits);
                    DeleteObject(hBitmap);
                    EndDoc(hDC);
                    DeleteDC(hDC);
                    return 0;
                }
                EndDoc(hDC);
                DeleteDC(hDC);
                GlobalFree(hInfo);
                GlobalFree(hBits);
                DeleteObject(hBitmap);
                return 1;
            }
        }
        _close(fd);
    }
    DeleteDC(hDC);
    return 0;
}

// FUNCTION: LEGOLAND 0x00451e20
int PrintCertificateWithCurrentDate(void) {
    time_t now;
    char *str;

    time(&now);
    str = asctime(localtime(&now));
    str[strlen(str) - 1] = '\0';
    // STRING: LEGOLAND 0x004b86fc
    return PrintCertificate("EGC.bmp", (char *)&CurrentProfile, str) != 0;
}
