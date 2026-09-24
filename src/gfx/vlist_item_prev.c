/* vlist_item_prev -- shipped at 1068:d637.
 *
 * Ported by tools/port.py from matched/gfx/vlist_item_prev.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * vlist_item_prev(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x8)));
}
