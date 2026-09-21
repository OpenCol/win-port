/* get_p_2e_1070_99c5 -- shipped at 1070:99c5.
 *
 * Ported by tools/port.py from matched/platform/get_p_2e_1070_99c5.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_2e_1070_99c5(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x2e)));
}
