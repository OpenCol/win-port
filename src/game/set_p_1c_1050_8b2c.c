/* set_p_1c_1050_8b2c -- shipped at 1050:8b2c.
 *
 * Ported by tools/port.py from matched/game/set_p_1c_1050_8b2c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1c_1050_8b2c(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1c))) = h;
}
