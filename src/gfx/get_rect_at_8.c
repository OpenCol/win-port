/* get_rect_at_8 -- shipped at 1068:4e88.
 *
 * Ported by tools/port.py from matched/gfx/get_rect_at_8.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void get_rect_at_8(Rec *o, RECT *r)
{
    *r = (*((RECT *)((unsigned char *)(o) + 0x8)));
}
