/* set_window_layout -- shipped at 1048:06e0.
 *
 * Ported by tools/port.py from matched/game/set_window_layout.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ctl_move(void *o, col_i16 a, col_i16 b);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void win_clamp_client_size(Win *w, col_i16 cx, col_i16 cy);

/* same object: 4-byte `push cs; call ` */


void set_window_layout(Win *w, register col_i16 cx, register col_i16 cy)
{
    RECT r;

    cx = cx + 0x14;
    cy = cy + 0x2a;
    SetRect(&r, cx - 0x20, 0, cx, 0x16);
    ctl_move(((char *)((unsigned char *)(w) + 0x6ea)), r.left, r.top);
    SetRect(&(*((RECT *)((unsigned char *)(w) + 0x76a))), 0xa, 0x20, cx - 0xa, cy - 0xa);
    win_clamp_client_size(w, cx, cy);
}
