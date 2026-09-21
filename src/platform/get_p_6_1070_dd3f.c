/* get_p_6_1070_dd3f -- shipped at 1070:dd3f.
 *
 * Ported by tools/port.py from matched/platform/get_p_6_1070_dd3f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_6_1070_dd3f(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x6)));
}
