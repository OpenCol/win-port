/* GRPort_GetWin_1070_960d -- shipped at 1070:960d.
 *
 * Ported by tools/port.py from matched/platform/GRPort_GetWin_1070_960d.c.
 *
 * The file's own MSWin, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

MSWin * GRPort_GetWin_1070_960d(GRPort *p)
{
    return (*((MSWin * *)((unsigned char *)(p) + 0x22)));
}
