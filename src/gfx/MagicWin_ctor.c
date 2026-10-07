/* MagicWin_ctor: candidate for 1068:a698 (115 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin::MagicWin()
 *
 * The constructor of MagicWin (0x100 bytes, over GameWin, vtable at DGROUP:0x3274) with a
 * GRPort member at +0xc4: clears the byte at +0xfa and the long at +0xfc.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

MagicWin::MagicWin()
{
    bound = 0;
    anim = 0;
}
