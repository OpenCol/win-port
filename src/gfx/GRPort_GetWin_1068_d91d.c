/* GRPort_GetWin_1068_d91d -- shipped at 1068:d91d.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_GetWin_1068_d91d.c.
 *
 * The file's own Win, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

Win * GRPort_GetWin_1068_d91d(GRPort *p)
{
    return (*((Win * *)((unsigned char *)(p) + 0x22)));
}
