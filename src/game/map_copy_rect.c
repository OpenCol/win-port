/* map_copy_rect -- shipped at 1038:a620.
 *
 * Ported by tools/port.py from matched/game/map_copy_rect.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_backdrop_port[];                   SEG28:0x0000
 *   extern int g_stub_a;                                 
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_is_empty(RECT *r);
void clamp_rect_to_window(void *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal CopyRect(RECT *dst, RECT *src);
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);




void map_copy_rect(MapWin *w, col_i16 x, col_i16 y, col_i16 wd, col_i16 ht)
{
    RECT dst;
    RECT src;

    SetRect(&src, x, y, x + wd, y + ht);
    clamp_rect_to_window(w, &src);
    CopyRect(&dst, &src);
    OffsetRect(&dst, w->ox, w->oy);
    if (rect_is_empty(&dst) == 0 && rect_is_empty(&src) == 0)
        CopyToPort(g_backdrop_port, w, &src, &dst);
}
