/* get_p_6_1040_aee6 -- shipped at 1040:aee6.
 *
 * Ported by tools/port.py from matched/game/get_p_6_1040_aee6.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_6_1040_aee6(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x6)));
}
