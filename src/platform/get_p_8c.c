/* get_p_8c -- shipped at 1070:a04b.
 *
 * Ported by tools/port.py from matched/platform/get_p_8c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_8c(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x8c)));
}
