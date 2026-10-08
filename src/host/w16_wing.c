/* WinG on Win32 (roadmap W4, section 1: "WinG -> CreateDIBSection plus BitBlt/StretchBlt").
 *
 * WinG gave Windows 3.1 games a fast 8-bit DIB they could write directly: WinGCreateDC, a
 * bitmap from WinGCreateBitmap selected into it, its bits from WinGGetDIBPointer, its colour
 * table set with WinGSetDIBColorTable, and WinGBitBlt/WinGStretchBlt to the screen. A DIB
 * section is all of that: an 8-bit DIB with its own colour table, whose pixels the program
 * owns, blitted through GDI. The game's BITMAPINFO is Win32's with 256 colours (0x428 bytes).
 */
#include "w16.h"

#define TOK(h) w16_token((HANDLE)(h))

unsigned w16_WinGCreateDC(void) W16(WinGCreateDC);
unsigned w16_WinGCreateDC(void)
{
    return TOK(CreateCompatibleDC(0));
}

/* WinG recommended a top-down 8-bit DIB on most hardware; the game handles either */
int w16_WinGRecommendDIBFormat(BITMAPINFO *bi) W16(WinGRecommendDIBFormat);
int w16_WinGRecommendDIBFormat(BITMAPINFO *bi)
{
    if (!bi)
        return 0;
    bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi->bmiHeader.biWidth = 1;
    bi->bmiHeader.biHeight = -1;
    bi->bmiHeader.biPlanes = 1;
    bi->bmiHeader.biBitCount = 8;
    bi->bmiHeader.biCompression = BI_RGB;
    bi->bmiHeader.biSizeImage = 0;
    bi->bmiHeader.biXPelsPerMeter = 0;
    bi->bmiHeader.biYPelsPerMeter = 0;
    bi->bmiHeader.biClrUsed = 0;
    bi->bmiHeader.biClrImportant = 0;
    return 1;
}

unsigned w16_WinGCreateBitmap(unsigned hdc, BITMAPINFO *bi, void **bits) W16(WinGCreateBitmap);
unsigned w16_WinGCreateBitmap(unsigned hdc, BITMAPINFO *bi, void **bits)
{
    void *p = 0;
    HBITMAP b = CreateDIBSection(W16_HDC(hdc), bi, DIB_RGB_COLORS, &p, 0, 0);
    if (bits)
        *bits = p;
    w16_trace("WinGCreateBitmap(%ldx%ld) = %p, bits %p", bi->bmiHeader.biWidth, bi->bmiHeader.biHeight, (void *)b, p);
    return TOK(b);
}

void *w16_WinGGetDIBPointer(int hbm, BITMAPINFO *bi) W16(WinGGetDIBPointer);
void *w16_WinGGetDIBPointer(int hbm, BITMAPINFO *bi)
{
    DIBSECTION ds;
    int n = GetObjectA((HBITMAP)w16_handle((unsigned)hbm), sizeof ds, &ds);
    w16_trace("WinGGetDIBPointer(%#x -> %p) = %d bytes, bits %p", (unsigned)hbm, w16_handle((unsigned)hbm), n,
              n == sizeof ds ? ds.dsBm.bmBits : 0);
    if (n != sizeof ds)
        return 0;
    if (bi) {
        bi->bmiHeader = ds.dsBmih;
    }
    return ds.dsBm.bmBits;
}

unsigned w16_WinGSetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q) W16(WinGSetDIBColorTable);
unsigned w16_WinGSetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q)
{
    return SetDIBColorTable(W16_HDC(hdc), start, n, q);
}

unsigned w16_WinGGetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q) W16(WinGGetDIBColorTable);
unsigned w16_WinGGetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q)
{
    return GetDIBColorTable(W16_HDC(hdc), start, n, q);
}

int w16_WinGBitBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy) W16(WinGBitBlt);
int w16_WinGBitBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy)
{
    return BitBlt(W16_HDC(d), (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, W16_HDC(s), (int16_t)sx, (int16_t)sy,
                  SRCCOPY);
}

int w16_WinGStretchBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy, int sw, int sh) W16(WinGStretchBlt);
int w16_WinGStretchBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy, int sw, int sh)
{
    HDC dc = W16_HDC(d);
    SetStretchBltMode(dc, COLORONCOLOR);
    return StretchBlt(dc, (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, W16_HDC(s), (int16_t)sx, (int16_t)sy,
                      (int16_t)sw, (int16_t)sh, SRCCOPY);
}
