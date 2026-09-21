/* set_p_22_1050_89cd -- shipped at 1050:89cd.
 *
 * Ported by tools/port.py from matched/game/set_p_22_1050_89cd.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_22_1050_89cd(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x22))) = h;
}
