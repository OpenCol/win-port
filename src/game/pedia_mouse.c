/* pedia_mouse -- shipped at 1060:3153.
 *
 * Ported by tools/port.py from matched/game/pedia_mouse.c.
 *
 * The file's own PediaWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: oy -> top
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void pedia_click(PediaWin *p, col_i16 x, col_i16 y);
void win_take_focus(void *w);

extern void * current_window(void); /* 1060:9fcd */

/* same object: 4-byte `push cs; call ` */



void pedia_mouse(register col_i16 x, register col_i16 y)
{
    PediaWin *p;
    void *w;

    w = current_window();
    p = (PediaWin *)(w != 0 ? (char *)w - 0x26 : (char *)w);
    if (p->left > x || p->right < x || p->top > y || p->bottom < y) {
        win_take_focus(((char *)((unsigned char *)(p) + 0x26)));
        return;
    }
    win_take_focus(((char *)((unsigned char *)(p) + 0x26)));
    pedia_click(p, x, y);
}
