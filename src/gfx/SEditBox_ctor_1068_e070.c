/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* SEditBox_ctor_1068_e070: candidate for 1068:e070 (110 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SEditBox::SEditBox()
 *
 * The edit-box constructor (0x28 bytes): the DlgControl base (its constructor defined here
 * for the `push cs / call ` form), three longs (+0x1a, +0x24, +0x1e) and a byte (+0x22)
 * cleared.
 */

#include "colonization.h"

#include "w16types.h"


#include "classes.h"

SEditBox::SEditBox()
{
    on_change = 0;
    f24 = 0;
    key_filter = 0;
    multiline = 0;
}
