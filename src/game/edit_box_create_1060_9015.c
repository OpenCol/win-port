/* edit_box_create_1060_9015 -- shipped at 1060:9015.
 *
 * Ported by tools/port.py from matched/game/edit_box_create_1060_9015.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own EditBox dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void edit_box_init_1060_a95b(EditBox *o, void *win, col_i16 id, RECT *r, void *text);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

void edit_box_create_1060_9015(EditBox *o, void *win, col_i16 id, col_i16 x, col_i16 y, col_i16 w, void *text)
{
    RECT r;

    o->f1a = 0;
    SetRect(&r, x, y, x + w, y + 0x1e);
    edit_box_init_1060_a95b(o, win, id, &r, text);
}
