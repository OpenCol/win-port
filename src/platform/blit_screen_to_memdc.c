/* blit_screen_to_memdc -- shipped at 1070:ca01.
 *
 * Ported by tools/port.py from matched/platform/blit_screen_to_memdc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height(RECT *r);
col_i16 rect_width(RECT *r);
void empty_1070_dc82(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal BitBlt(col_u16 dst, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_u16 src, col_i16 sx, col_i16 sy, col_u32 rop);
extern col_i16 pascal DeleteDC(col_u16 hdc);
extern col_i16 pascal DeleteObject(col_u16 hobj);
extern col_u16 pascal CreateCompatibleBitmap(col_u16 hdc, col_i16 w, col_i16 h);
extern col_u16 pascal CreateCompatibleDC(col_u16 hdc);
extern col_u16 pascal CreateDC(const char *driver, const char *device, const char *output, void *init);
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 hobj);
#endif

/* same object: 4-byte `push cs; call ` (rule 47) */










void blit_screen_to_memdc(Win *w, RECT *r)
{
    col_u16 hbm;
    col_i16 width;
    col_i16 height;
    col_u16 screen;
    col_u16 mem;

    width = rect_width(r);
    height = rect_height(r);
    screen = CreateDC("DISPLAY", 0, 0, 0);
    mem = CreateCompatibleDC(screen);
    hbm = CreateCompatibleBitmap(screen, width, height);
    SelectObject(mem, hbm);
    if (!BitBlt(mem, 0, 0, width, height, screen, r->left, r->top, 0xcc0020L))
        empty_1070_dc82(&(*g_error_ctx), "BitBlt error: copy screen to memDC");
    if (!BitBlt(w->hdc, 0, 0, width, height, mem, 0, 0, 0xcc0020L))
        empty_1070_dc82(&(*g_error_ctx), "BitBlt error: copy memDC to WinGDC");
    DeleteDC(mem);
    DeleteDC(screen);
    DeleteObject(hbm);
}
