/* set_p_1a_1068_e03c -- shipped at 1068:e03c.
 *
 * Ported by tools/port.py from matched/gfx/set_p_1a_1068_e03c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1a_1068_e03c(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1a))) = h;
}
