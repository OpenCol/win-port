/* vlist_item_next -- shipped at 1068:d61a.
 *
 * Ported by tools/port.py from matched/gfx/vlist_item_next.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * vlist_item_next(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x4)));
}
