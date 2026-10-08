/* The display the game was written for: 256 colours through a hardware palette (roadmap W4).
 *
 * In 1995 the game ran on an 8-bit palette display, and it asks (palette_init:
 * GetDeviceCaps(RASTERCAPS) & RC_PALETTE): with one it creates a palette and animates it --
 * the opening's fade is AnimatePalette, not new pixels. A modern display has no palette, so
 * this file is one in software:
 *   - GetDeviceCaps answers as a 256-colour palette device (RC_PALETTE, 8 bits per pixel, a
 *     256-entry palette, 20 static colours);
 *   - the system palette is an array here: the 20 static colours Windows reserves, then
 *     whatever RealizePalette, SetPaletteEntries and AnimatePalette put in it;
 *   - a WinG blit to a window shows the bitmap through it, the pixel being a palette index,
 *     as the hardware did -- and when the palette changes, the last blit to each window is made
 *     again, so the colours on the screen change as they did on the hardware ("palette
 *     animation by re-blit", roadmap section 1).
 * $PORT_TRUECOLOR=1 reports the display as it is (the game then takes its true-colour path).
 */
#include <stdlib.h>
#include <string.h>
#include "w16.h"

static int truecolor = -1;
static RGBQUAD sys[256];
static int sys_ready;

int w16_palette_device(void)
{
    if (truecolor < 0)
        truecolor = getenv("PORT_TRUECOLOR") != 0;
    return !truecolor;
}

/* Windows' 20 static colours: 0-9 and 246-255 */
static const unsigned char statics[20][3] = {
    {0, 0, 0}, {128, 0, 0}, {0, 128, 0}, {128, 128, 0}, {0, 0, 128}, {128, 0, 128}, {0, 128, 128},
    {192, 192, 192}, {192, 220, 192}, {166, 202, 240}, {255, 251, 240}, {160, 160, 164},
    {128, 128, 128}, {255, 0, 0}, {0, 255, 0}, {255, 255, 0}, {0, 0, 255}, {255, 0, 255},
    {0, 255, 255}, {255, 255, 255}};

static void sys_init(void)
{
    int i;
    if (sys_ready)
        return;
    sys_ready = 1;
    for (i = 0; i < 20; i++) {
        int at = i < 10 ? i : 236 + i;
        sys[at].rgbRed = statics[i][0];
        sys[at].rgbGreen = statics[i][1];
        sys[at].rgbBlue = statics[i][2];
    }
}

int w16_device_caps(HDC dc, int i)
{
    if (w16_palette_device()) {
        switch (i) {
        case RASTERCAPS:
            return GetDeviceCaps(dc, RASTERCAPS) | RC_PALETTE;
        case BITSPIXEL:
            return 8;
        case PLANES:
            return 1;
        case NUMCOLORS:
            return 20;
        case SIZEPALETTE:
            return 256;
        case NUMRESERVED:
            return 20;
        case COLORRES:
            return 18;
        }
    }
    return GetDeviceCaps(dc, i);
}

unsigned w16_system_palette(unsigned start, unsigned n, PALETTEENTRY *out)
{
    unsigned i;
    sys_init();
    for (i = 0; i < n && start + i < 256; i++) {
        out[i].peRed = sys[start + i].rgbRed;
        out[i].peGreen = sys[start + i].rgbGreen;
        out[i].peBlue = sys[start + i].rgbBlue;
        out[i].peFlags = 0;
    }
    return i;
}

/* ------------------------------------------------------------------ the last blit to each window */

typedef struct { HWND hwnd; HDC src; int x, y, w, h, sx, sy, sw, sh; } Blit;
static Blit last[32];

static void redo(void);

/* a palette entry of the system palette changed: what is on the screen shows it */
static void sys_changed(void)
{
    redo();
}

/* the palette the game realized: its non-static entries become the system palette's */
void w16_realize(HPALETTE pal)
{
    PALETTEENTRY e[256];
    unsigned i, n;
    sys_init();
    n = GetPaletteEntries(pal, 0, 256, e);
    for (i = 10; i < n && i < 246; i++) {
        sys[i].rgbRed = e[i].peRed;
        sys[i].rgbGreen = e[i].peGreen;
        sys[i].rgbBlue = e[i].peBlue;
    }
    sys_changed();
}

void w16_animate(unsigned start, unsigned n, const PALETTEENTRY *e)
{
    unsigned i;
    sys_init();
    for (i = 0; i < n && start + i < 256; i++) {
        if (start + i < 10 || start + i >= 246)
            continue;                     /* the static colours stay */
        sys[start + i].rgbRed = e[i].peRed;
        sys[start + i].rgbGreen = e[i].peGreen;
        sys[start + i].rgbBlue = e[i].peBlue;
    }
    sys_changed();
}

/* a blit from an 8-bit DIB to a window: through the system palette, its pixels indices */
int w16_display_blit(HDC dst, int x, int y, int w, int h, HDC src, int sx, int sy, int sw, int sh)
{
    RGBQUAD own[256];
    unsigned n = 0;
    int r;
    HWND hw = WindowFromDC(dst);
    if (w16_palette_device()) {
        sys_init();
        n = GetDIBColorTable(src, 0, 256, own);
        if (n)
            SetDIBColorTable(src, 0, 256, sys);
    }
    if (sw == w && sh == h)
        r = BitBlt(dst, x, y, w, h, src, sx, sy, SRCCOPY);
    else {
        SetStretchBltMode(dst, COLORONCOLOR);
        r = StretchBlt(dst, x, y, w, h, src, sx, sy, sw, sh, SRCCOPY);
    }
    if (n)
        SetDIBColorTable(src, 0, n, own);
    if (hw) {
        int i, free_slot = -1;
        for (i = 0; i < 32; i++) {
            if (last[i].hwnd == hw && last[i].src == src && last[i].x == x && last[i].y == y)
                break;
            if (free_slot < 0 && (!last[i].hwnd || !IsWindow(last[i].hwnd)))
                free_slot = i;
        }
        if (i == 32)
            i = free_slot >= 0 ? free_slot : 0;
        last[i].hwnd = hw; last[i].src = src; last[i].x = x; last[i].y = y; last[i].w = w; last[i].h = h;
        last[i].sx = sx; last[i].sy = sy; last[i].sw = sw; last[i].sh = sh;
    }
    return r;
}

static void redo(void)
{
    int i;
    static int busy;
    if (busy || !w16_palette_device())
        return;
    busy = 1;
    for (i = 0; i < 32; i++) {
        Blit b = last[i];
        HDC dc;
        if (!b.hwnd || !IsWindow(b.hwnd))
            continue;
        dc = GetDC(b.hwnd);
        w16_display_blit(dc, b.x, b.y, b.w, b.h, b.src, b.sx, b.sy, b.sw, b.sh);
        ReleaseDC(b.hwnd, dc);
    }
    busy = 0;
}
