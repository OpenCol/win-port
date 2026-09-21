/* get_p_4_1070_96a8 -- shipped at 1070:96a8.
 *
 * Ported by tools/port.py from matched/platform/get_p_4_1070_96a8.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_4_1070_96a8(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x4)));
}
