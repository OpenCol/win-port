/* render_1070_8e7a -- shipped at 1070:8e7a.
 *
 * Ported by tools/port.py from matched/platform/render_1070_8e7a.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_palette_mode;                       DGROUP:0x3380
 *
 * The file's own MSWin, WinPal dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1070_9649(RECT *r);
col_i16 rect_width_1070_962a(RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ReleaseDC(col_u16 hwnd, col_u16 hdc);
extern col_i16 pascal WinGBitBlt(col_u16 dst, col_i16 dx, col_i16 dy, col_i16 w, col_i16 h,
                                 col_u16 src, col_i16 sx, col_i16 sy);
extern col_u16 pascal GetDC(col_u16 hwnd);
extern col_u16 pascal RealizePalette(col_u16 hdc);
extern col_u16 pascal SelectPalette(col_u16 hdc, col_u16 hpal, col_i16 force);
#endif

/* same object: 4-byte `push cs; call ` */



void render_1070_8e7a(MSWin *src, WinPal *win, RECT *sr, RECT *dr)
{
    col_u16 dc;

    if (win != 0 && src != 0) {
        dc = GetDC(win->hwnd);
        if (win->hpal != 0 && g_palette_mode == 1) {
            SelectPalette(dc, win->hpal, 0);
            RealizePalette(dc);
        }
        WinGBitBlt(dc, dr->left, dr->top, rect_width_1070_962a(sr), rect_height_1070_9649(sr),
                   src->hwnd, sr->left, sr->top);
        ReleaseDC(win->hwnd, dc);
    }
}
