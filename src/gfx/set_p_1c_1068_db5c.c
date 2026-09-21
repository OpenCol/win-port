/* set_p_1c_1068_db5c -- shipped at 1068:db5c.
 *
 * Ported by tools/port.py from matched/gfx/set_p_1c_1068_db5c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1c_1068_db5c(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1c))) = h;
}
