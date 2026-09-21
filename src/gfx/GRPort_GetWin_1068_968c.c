/* GRPort_GetWin_1068_968c -- shipped at 1068:968c.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_GetWin_1068_968c.c.
 *
 * The file's own Win, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

Win * GRPort_GetWin_1068_968c(GRPort *p)
{
    return (*((void * *)((unsigned char *)(p) + 0x22)));
}
