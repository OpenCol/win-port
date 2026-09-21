/* set_p_1a_1048_d65a -- shipped at 1048:d65a.
 *
 * Ported by tools/port.py from matched/game/set_p_1a_1048_d65a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1a_1048_d65a(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1a))) = h;
}
