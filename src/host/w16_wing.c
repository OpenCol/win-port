/* WinG on Win32 (roadmap W4, section 1: "WinG -> CreateDIBSection plus BitBlt/StretchBlt").
 *
 * WinG gave Windows 3.1 games a fast 8-bit DIB they could write directly: WinGCreateDC, a
 * bitmap from WinGCreateBitmap selected into it, its bits from WinGGetDIBPointer, its colour
 * table set with WinGSetDIBColorTable, and WinGBitBlt/WinGStretchBlt to the screen. A DIB
 * section is all of that: an 8-bit DIB with its own colour table, whose pixels the program
 * owns, blitted through GDI. The game's BITMAPINFO is Win32's with 256 colours (0x428 bytes).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    HBITMAP b;
    if ((long long)bi->bmiHeader.biWidth * bi->bmiHeader.biHeight >= 16ll << 20
        || (long long)bi->bmiHeader.biWidth * bi->bmiHeader.biHeight <= -(16ll << 20) || bi->bmiHeader.biWidth < 0)
        w16_report("huge WinGCreateBitmap(%ld x %ld) from %p", bi->bmiHeader.biWidth, bi->bmiHeader.biHeight,
                   __builtin_return_address(0));
    b = CreateDIBSection(W16_HDC(hdc), bi, DIB_RGB_COLORS, &p, 0, 0);
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

/* $PORT_SNAPSHOT (a directory): what the game puts in its windows, as frame.bmp and a
   numbered copy every 3 seconds -- for a run nobody watches. Every WinG blit to a window is
   also made into a 32-bit "screen" the size of the largest window, at the window's client
   coordinates; that is what is saved. */
static HDC shadow_dc;
static HBITMAP shadow_bm;
static void *shadow_bits;
#define SHADOW_W 1024
#define SHADOW_H 768

/* the shadow screen to a .bmp file (0 when $PORT_SNAPSHOT is not set): a lockstep session's
   `snap` (w16_lockstep.c) and the frames above */
int w16_snapshot_write(const char *path)
{
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER bh;
    DWORD size = SHADOW_W * SHADOW_H * 4, wr;
    HANDLE f;
    if (!shadow_dc)
        return 0;
    GdiFlush();
    f = CreateFileA(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (f == INVALID_HANDLE_VALUE)
        return 0;
    memset(&bh, 0, sizeof bh);
    bh.biSize = sizeof bh;
    bh.biWidth = SHADOW_W;
    bh.biHeight = -SHADOW_H;
    bh.biPlanes = 1;
    bh.biBitCount = 32;
    bh.biSizeImage = size;
    fh.bfType = 0x4d42;
    fh.bfOffBits = sizeof fh + sizeof bh;
    fh.bfSize = fh.bfOffBits + size;
    fh.bfReserved1 = fh.bfReserved2 = 0;
    WriteFile(f, &fh, sizeof fh, &wr, 0);
    WriteFile(f, &bh, sizeof bh, &wr, 0);
    WriteFile(f, shadow_bits, size, &wr, 0);
    CloseHandle(f);
    return 1;
}

static void shadow_blit(HDC dst, int x, int y, int w, int h, HDC src, int sx, int sy, int sw, int sh)
{
    static const char *dir;
    static int checked;
    static DWORD last;
    static unsigned n;
    char path[MAX_PATH];
    if (!checked) {
        dir = getenv("PORT_SNAPSHOT");
        checked = 1;
        if (dir) {
            BITMAPINFO bi;
            memset(&bi, 0, sizeof bi);
            bi.bmiHeader.biSize = sizeof bi.bmiHeader;
            bi.bmiHeader.biWidth = SHADOW_W;
            bi.bmiHeader.biHeight = -SHADOW_H;
            bi.bmiHeader.biPlanes = 1;
            bi.bmiHeader.biBitCount = 32;
            shadow_dc = CreateCompatibleDC(0);
            shadow_bm = CreateDIBSection(0, &bi, DIB_RGB_COLORS, &shadow_bits, 0, 0);
            SelectObject(shadow_dc, shadow_bm);
        }
    }
    if (!dir || !shadow_dc)
        return;
    {
        /* at the window's place, measured from the top-level window it belongs to */
        HWND hw = WindowFromDC(dst), top = hw ? GetAncestor(hw, GA_ROOT) : 0;
        POINT p = { 0, 0 }, o = { 0, 0 };
        if (hw)
            ClientToScreen(hw, &p);
        if (top) {
            static HWND first;
            if (!first || !IsWindow(first))
                first = top;
            ClientToScreen(first, &o);
        }
        x += p.x - o.x;
        y += p.y - o.y;
    }
    if (sw == w && sh == h)
        BitBlt(shadow_dc, x, y, w, h, src, sx, sy, SRCCOPY);
    else
        StretchBlt(shadow_dc, x, y, w, h, src, sx, sy, sw, sh, SRCCOPY);
    GdiFlush();
    {
        DWORD now = GetTickCount();
        snprintf(path, sizeof path, "%s/frame.bmp", dir);
        w16_snapshot_write(path);
        if (now - last >= 3000) {
            snprintf(path, sizeof path, "%s/frame-%03u.bmp", dir, n++);
            last = now;
            w16_snapshot_write(path);
        }
    }
}

int w16_WinGBitBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy) W16(WinGBitBlt);
int w16_WinGBitBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy)
{
    shadow_blit(W16_HDC(d), (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, W16_HDC(s), (int16_t)sx, (int16_t)sy, (int16_t)w, (int16_t)h);
    return w16_display_blit(W16_HDC(d), (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, W16_HDC(s), (int16_t)sx,
                            (int16_t)sy, (int16_t)w, (int16_t)h);
}

int w16_WinGStretchBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy, int sw, int sh) W16(WinGStretchBlt);
int w16_WinGStretchBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy, int sw, int sh)
{
    HDC dc = W16_HDC(d);
    shadow_blit(dc, (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, W16_HDC(s), (int16_t)sx, (int16_t)sy, (int16_t)sw, (int16_t)sh);
    return w16_display_blit(dc, (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, W16_HDC(s), (int16_t)sx, (int16_t)sy,
                            (int16_t)sw, (int16_t)sh);
}

/* what Wine shows: every visible top-level window of the program, read back from its window DC
   and placed at its screen position (bottom of the z-order first) -- a lockstep script's `grab`.
   Unlike the shadow screen above, this is after the display path (w16_display.c): the system
   palette, GDI text, the menu bar, the frames. */
static HWND grab_list[64];
static int grab_n;

static BOOL CALLBACK grab_collect(HWND h, LPARAM l)
{
    DWORD pid;
    (void)l;
    GetWindowThreadProcessId(h, &pid);
    if (pid == GetCurrentProcessId() && IsWindowVisible(h) && grab_n < 64)
        grab_list[grab_n++] = h;
    return TRUE;
}

static void grab_describe(FILE *t, HWND h, int depth)
{
    char cls[64], title[64];
    RECT r;
    HWND c;
    GetClassNameA(h, cls, sizeof cls);
    GetWindowTextA(h, title, sizeof title);
    GetWindowRect(h, &r);
    fprintf(t, "%*s%p %s \"%s\" (%ld,%ld)-(%ld,%ld) style %08lx%s\n", depth * 2, "", (void *)h, cls, title, r.left, r.top,
            r.right, r.bottom, (unsigned long)GetWindowLongA(h, GWL_STYLE), IsWindowVisible(h) ? "" : " hidden");
    for (c = GetWindow(h, GW_CHILD); c && depth < 4; c = GetWindow(c, GW_HWNDNEXT))
        if (IsWindowVisible(c))
            grab_describe(t, c, depth + 1);
}

/* the top-left corner of a grab: the union of the program's visible top-level windows */
static int grab_origin(POINT *o)
{
    RECT all = { 0x7fff, 0x7fff, -0x7fff, -0x7fff }, r;
    int i;
    grab_n = 0;
    EnumWindows(grab_collect, 0);
    for (i = 0; i < grab_n; i++) {
        GetWindowRect(grab_list[i], &r);
        UnionRect(&all, &all, &r);
    }
    o->x = all.left;
    o->y = all.top;
    return grab_n > 0;
}

/* a mouse click at (x, y) of the topmost window of the program (its window rectangle, frame
   included): down and up to the window under it, in its client coordinates -- a lockstep
   script's `click`. The topmost window, not the grab's corner: a realtime run's backdrop covers
   the host's whole screen and moves the corner */
HWND w16_grab_click(int x, int y, POINT *client)
{
    POINT o, p;
    RECT r;
    HWND h;
    client->x = client->y = 0;
    if (!grab_origin(&o))
        return 0;
    GetWindowRect(grab_list[0], &r);
    o.x = r.left;
    o.y = r.top;
    p.x = o.x + x;
    p.y = o.y + y;
    if (!(h = WindowFromPoint(p)))
        return 0;
    SetCursorPos(p.x, p.y);
    ScreenToClient(h, &p);
    *client = p;
    PostMessageA(h, WM_MOUSEMOVE, 0, MAKELPARAM(p.x, p.y));
    PostMessageA(h, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(p.x, p.y));
    PostMessageA(h, WM_LBUTTONUP, 0, MAKELPARAM(p.x, p.y));
    return h;
}

int w16_grab_write(const char *path)
{
    RECT all = { 0x7fff, 0x7fff, -0x7fff, -0x7fff }, r;
    BITMAPINFO bi;
    BITMAPFILEHEADER fh;
    HDC dc;
    HBITMAP bm;
    void *bits;
    DWORD size, wr;
    HANDLE f;
    int i, w, h;
    grab_n = 0;
    EnumWindows(grab_collect, 0);
    for (i = 0; i < grab_n; i++) {
        GetWindowRect(grab_list[i], &r);
        UnionRect(&all, &all, &r);
    }
    if (!grab_n || all.right <= all.left || all.bottom <= all.top)
        return 0;
    w = all.right - all.left;
    h = all.bottom - all.top;
    memset(&bi, 0, sizeof bi);
    bi.bmiHeader.biSize = sizeof bi.bmiHeader;
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = -h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    dc = CreateCompatibleDC(0);
    bm = CreateDIBSection(0, &bi, DIB_RGB_COLORS, &bits, 0, 0);
    SelectObject(dc, bm);
    for (i = grab_n - 1; i >= 0; i--) {
        HDC wdc = GetWindowDC(grab_list[i]);
        GetWindowRect(grab_list[i], &r);
        BitBlt(dc, r.left - all.left, r.top - all.top, r.right - r.left, r.bottom - r.top, wdc, 0, 0, SRCCOPY);
        ReleaseDC(grab_list[i], wdc);
    }
    GdiFlush();
    {   /* beside it, NAME.txt: the windows, top of the z-order first, and their visible children */
        char txt[MAX_PATH + 8];
        FILE *t;
        snprintf(txt, sizeof txt, "%.*s.txt", (int)(strlen(path) - 4), path);
        if ((t = fopen(txt, "w"))) {
            for (i = 0; i < grab_n; i++)
                grab_describe(t, grab_list[i], 0);
            fclose(t);
        }
    }
    size = (DWORD)w * h * 4;
    f = CreateFileA(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (f != INVALID_HANDLE_VALUE) {
        fh.bfType = 0x4d42;
        fh.bfOffBits = sizeof fh + sizeof bi.bmiHeader;
        fh.bfSize = fh.bfOffBits + size;
        fh.bfReserved1 = fh.bfReserved2 = 0;
        bi.bmiHeader.biSizeImage = size;
        WriteFile(f, &fh, sizeof fh, &wr, 0);
        WriteFile(f, &bi.bmiHeader, sizeof bi.bmiHeader, &wr, 0);
        WriteFile(f, bits, size, &wr, 0);
        CloseHandle(f);
    }
    DeleteDC(dc);
    DeleteObject(bm);
    return f != INVALID_HANDLE_VALUE;
}
