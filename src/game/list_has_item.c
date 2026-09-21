/* list_has_item -- shipped at 1060:911b.
 *
 * Ported by tools/port.py from matched/game/list_has_item.c.
 *
 * The file's own ListItem dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x92 bytes */
typedef struct { char pad[0x26]; col_i16 count; char pad2[6]; ListItem *items; } ListWin;

col_i16 list_has_item(ListWin *o, col_i16 key)
{
    col_i16 i;

    for (i = 0; o->count > i; i++) {
        if (o->items[i].hwnd == key)
            return 1;
    }
    return 0;
}
