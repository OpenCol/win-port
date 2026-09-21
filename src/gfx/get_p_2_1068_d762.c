/* get_p_2_1068_d762 -- shipped at 1068:d762.
 *
 * Ported by tools/port.py from matched/gfx/get_p_2_1068_d762.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2_1068_d762(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2)));
}
