/* get_p_18_1068_989f -- shipped at 1068:989f.
 *
 * Ported by tools/port.py from matched/gfx/get_p_18_1068_989f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_18_1068_989f(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x18)));
}
