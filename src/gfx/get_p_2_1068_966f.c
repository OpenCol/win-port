/* get_p_2_1068_966f -- shipped at 1068:966f.
 *
 * Ported by tools/port.py from matched/gfx/get_p_2_1068_966f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2_1068_966f(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2)));
}
