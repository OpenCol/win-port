/* GRPort_GetWin_1068_d91d: candidate for 1068:d91d (29 bytes).  Verdict: EXACT.
 * size-source: 29, not the census 25. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * The port's window (pointer at +0x22); module 1068's second copy, byte for byte the
 * same as `GRPort_GetWin_1068_968c` 0x4291 (17,041) bytes above it. Two copies of a
 * header-defined accessor in one segment is two translation units, not two functions.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

GrafRec *GRPort_GetWin_1068_d91d(GRPort *p)
{
    return (GrafRec *)p->win;
}
