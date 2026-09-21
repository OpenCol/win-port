/* get_p_2_1058_c3d9 -- shipped at 1058:c3d9.
 *
 * Ported by tools/port.py from matched/game/get_p_2_1058_c3d9.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2_1058_c3d9(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2)));
}
