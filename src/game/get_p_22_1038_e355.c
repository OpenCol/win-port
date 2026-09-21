/* get_p_22_1038_e355 -- shipped at 1038:e355.
 *
 * Ported by tools/port.py from matched/game/get_p_22_1038_e355.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_22_1038_e355(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x22)));
}
