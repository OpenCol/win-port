/* get_p_6_1038_e372 -- shipped at 1038:e372.
 *
 * Ported by tools/port.py from matched/game/get_p_6_1038_e372.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_6_1038_e372(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x6)));
}
