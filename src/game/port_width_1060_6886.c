/* port_width_1060_6886: candidate for 1060:6886 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `port_width_1060_65ca`, named for the routine it reaches.
 *
 * RETURN TYPE CORRECTED. This file was `port_width_1060_6886` returning `void *`, which
 * is the §119 blind spot: a pointer and an `int` both come back in DX:AX and the
 * comparator cannot tell them apart. Two routes settle it. The callee reads one WORD at +2
 * and returns it in AX alone, and `PediaWin_DrawTitle` -- the only caller that does
 * anything with the result -- follows the call with `CWD / SUB AX,DX / SAR AX,1`, which is
 * the signed divide-by-two of an `int`. A pointer's high half would be clobbered by the
 * `CWD`.
 *
 * The callee is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern int port_width_1060_65ca(GRPort *port);

int port_width_1060_6886(GRPort *p)
{
    return port_width_1060_65ca(p);
}
