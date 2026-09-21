/* set_p_1c_1030_e5bf -- shipped at 1030:e5bf.
 *
 * Ported by tools/port.py from matched/game/set_p_1c_1030_e5bf.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1c_1030_e5bf(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1c))) = h;
}
