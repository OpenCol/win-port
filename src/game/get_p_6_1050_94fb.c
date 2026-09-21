/* get_p_6_1050_94fb -- shipped at 1050:94fb.
 *
 * Ported by tools/port.py from matched/game/get_p_6_1050_94fb.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_6_1050_94fb(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x6)));
}
