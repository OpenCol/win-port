/* set_p_80_1048_e176 -- shipped at 1048:e176.
 *
 * Ported by tools/port.py from matched/game/set_p_80_1048_e176.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_80_1048_e176(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x80))) = h;
}
