/* list_show_items -- shipped at 1060:915b.
 *
 * Ported by tools/port.py from matched/game/list_show_items.c.
 *
 * The file's own ListItem dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x92 bytes */
typedef struct { char pad[0x26]; col_i16 count; char pad2[6]; ListItem *items; } ListWin;
extern void show_hwnd(col_i16 h);
extern void invalidate_hwnd(col_i16 h);

void list_show_items(ListWin *o)
{
    register col_i16 i;

    for (i = 0; o->count > i; i++) {
        show_hwnd(o->items[i].hwnd);
        invalidate_hwnd(o->items[i].hwnd);
    }
}
