/* set_p_22_1010_e93f -- shipped at 1010:e93f.
 *
 * Ported by tools/port.py from matched/game/set_p_22_1010_e93f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_22_1010_e93f(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x22))) = h;
}
