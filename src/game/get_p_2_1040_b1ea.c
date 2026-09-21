/* get_p_2_1040_b1ea -- shipped at 1040:b1ea.
 *
 * Ported by tools/port.py from matched/game/get_p_2_1040_b1ea.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2_1040_b1ea(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2)));
}
