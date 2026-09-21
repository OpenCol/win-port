/* set_p_1c_1010_eaf5 -- shipped at 1010:eaf5.
 *
 * Ported by tools/port.py from matched/game/set_p_1c_1010_eaf5.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1c_1010_eaf5(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1c))) = h;
}
