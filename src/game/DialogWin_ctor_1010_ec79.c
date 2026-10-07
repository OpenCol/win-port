/* DialogWin_ctor_1010_ec79: candidate for 1010:ec79 (115 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DialogWin::DialogWin()
 *
 * The constructor of DialogWin (0x9a bytes), derived from EventWin, as compiled into module
 * 1010: the base constructed through the module's EventWin copy, then three longs
 * zeroed and a byte set to 1.
 */

#include "colonization.h"

#include "classes.h"
class DlgControl;

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

DialogWin::DialogWin()
{
    first_ctl = 0;
    ok_btn = 0;
    cancel_btn = 0;
    keyboard_nav = 1;
}
