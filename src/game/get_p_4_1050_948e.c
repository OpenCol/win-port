/* get_p_4_1050_948e -- shipped at 1050:948e.
 *
 * Ported by tools/port.py from matched/game/get_p_4_1050_948e.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_4_1050_948e(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x4)));
}
