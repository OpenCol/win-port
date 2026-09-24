/* win_hide_1050_6e73 -- shipped at 1050:6e73.
 *
 * Ported by tools/port.py from matched/game/win_hide_1050_6e73.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSHideWindow_of_1050_8cd8(void *sub);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void win_click_to_port(Win *w, RECT *r);

/* same object: 4-byte `push cs; call ` */


void win_hide_1050_6e73(Win *w)
{
    RECT r;

    SetRect(&r, 0, 0, 1, 1);
    MSHideWindow_of_1050_8cd8(((char *)((unsigned char *)(w) + 0x26)));
    win_click_to_port(w, &r);
    (*((col_i16 *)((unsigned char *)(w) + 0xc4))) = -1;
}
