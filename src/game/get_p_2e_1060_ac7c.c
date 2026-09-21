/* get_p_2e_1060_ac7c -- shipped at 1060:ac7c.
 *
 * Ported by tools/port.py from matched/game/get_p_2e_1060_ac7c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2e_1060_ac7c(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2e)));
}
