/* get_p_6_1030_ed1f -- shipped at 1030:ed1f.
 *
 * Ported by tools/port.py from matched/game/get_p_6_1030_ed1f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_6_1030_ed1f(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x6)));
}
