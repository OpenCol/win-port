/* set_p_8 -- shipped at 1068:9cee.
 *
 * Ported by tools/port.py from matched/gfx/set_p_8.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_8(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x8))) = h;
}
