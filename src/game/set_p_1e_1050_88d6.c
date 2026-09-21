/* set_p_1e_1050_88d6 -- shipped at 1050:88d6.
 *
 * Ported by tools/port.py from matched/game/set_p_1e_1050_88d6.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1e_1050_88d6(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1e))) = h;
}
