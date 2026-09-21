/* list_hide_items -- shipped at 1060:91ad.
 *
 * Ported by tools/port.py from matched/game/list_hide_items.c.
 *
 * The file's own ListItem dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x92 bytes */
typedef struct { char pad[0x26]; col_i16 count; char pad2[6]; ListItem *items; } ListWin;
extern void hide_hwnd(col_i16 h);

void list_hide_items(ListWin *o)
{
    register col_i16 i;

    for (i = 0; o->count > i; i++)
        hide_hwnd(o->items[i].hwnd);
}
