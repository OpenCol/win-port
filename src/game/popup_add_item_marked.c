/* popup_add_item_marked -- shipped at 1050:13d7.
 *
 * Ported by tools/port.py from matched/game/popup_add_item_marked.c.
 *
 * The file's own Item, Popup dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Item * Popup_item(Popup *p, char *s, col_i16 id);

/* 0x12 bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */

Item * popup_add_item_marked(Popup *p, char *s, col_i16 id, col_i16 mark)
{
    Item *it;

    (*((col_i16 *)((unsigned char *)(p) + 0x0))) |= 5;
    it = Popup_item(p, s, id);
    if (mark != 0)
        (*((col_i16 *)((unsigned char *)(it) + 0x0))) |= 4;
    return it;
}
