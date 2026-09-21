/* set_p_c0_1048_d738 -- shipped at 1048:d738.
 *
 * Ported by tools/port.py from matched/game/set_p_c0_1048_d738.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_c0_1048_d738(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0xc0))) = h;
}
