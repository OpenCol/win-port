/* DebugWin_ctor: candidate for 1008:d3a0 (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DebugWin::DebugWin()
 *
 * The constructor of DebugWin (0xc4 bytes, over GameWin, with its own vtable at
 * DGROUP:0x150): nothing beyond the base construction and the vtable pointer.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

DebugWin::DebugWin()
{
}
