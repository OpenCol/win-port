/* get_p_2_1070_97b7 -- shipped at 1070:97b7.
 *
 * Ported by tools/port.py from matched/platform/get_p_2_1070_97b7.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2_1070_97b7(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2)));
}
