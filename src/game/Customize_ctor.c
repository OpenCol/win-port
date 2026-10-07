/* Customize_ctor: candidate for 1030:e9d2 (127 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize::Customize()
 *
 * The constructor of Customize (over GameWin, vtable at DGROUP:0x1368) with two SButton
 * members and a GRPort: nothing beyond the compiler's base and member construction (the
 * GameWin and SButton constructors are module 1030's copies).
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

Customize::Customize()
{
}
