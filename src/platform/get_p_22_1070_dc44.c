/* get_p_22_1070_dc44 -- shipped at 1070:dc44.
 *
 * Ported by tools/port.py from matched/platform/get_p_22_1070_dc44.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_22_1070_dc44(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x22)));
}
