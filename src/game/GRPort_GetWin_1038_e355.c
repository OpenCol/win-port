/* GRPort_GetWin_1038_e355: candidate for 1038:e355 (29 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x22 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_p_22_1038_e355: returns the window the port holds at +0x22 (MSLockGraf's GRPort.win) --
 * byte-identical to the three GRPort_GetWin copies at 1068:968c, 1068:d91d and 1070:960d.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void *GRPort_GetWin_1038_e355(GRPort *r)
{
    return r->win;
}
