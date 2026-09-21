/* get_p_2_1010_ee6f -- shipped at 1010:ee6f.
 *
 * Ported by tools/port.py from matched/game/get_p_2_1010_ee6f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2_1010_ee6f(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2)));
}
