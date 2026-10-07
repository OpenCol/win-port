/* DialogWin_ctor_1048_e757: candidate for 1048:e757 (115 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DialogWin::DialogWin()
 *
 * The constructor of DialogWin (0x9a bytes), derived from EventWin, as compiled into module
 * 1048: the base constructed through the module's EventWin copy, then three longs
 * zeroed and a byte set to 1.
 */

#include "colonization.h"

#include "classes.h"
class DlgControl;

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

/* PORT: this copy of DialogWin::DialogWin removed: src/game/DialogWin_ctor_1008_e502.c defines it once (W2). */
