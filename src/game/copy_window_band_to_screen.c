/* copy_window_band_to_screen -- shipped at 1048:1c15.
 *
 * Ported by tools/port.py from matched/game/copy_window_band_to_screen.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen(Win *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

/* 5-byte NOP form, so extern (rule 47) */
extern void set_current_nation(col_i16 n);
/* same object: 4-byte `push cs; call ` */


void copy_window_band_to_screen(Win *w)
{
    RECT r;

    set_current_nation(w->nation);
    SetRect(&r, 0, 8, 0x280, 0x102);
    OffsetRect(&r, w->ox, w->oy);
    copy_rect_to_screen(w, &r);
}
