/* SEditBox_ctor_1050_89e7: candidate for 1050:89e7 (99 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SEditBox::SEditBox()
 *
 * This module's own copy of the edit-box constructor: the DlgControl base, three longs
 * and a byte cleared. Byte-identical to the segment 1068 copy but for the fixups.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SEditBox::SEditBox()
{
    on_change = 0;
    f24 = 0;
    key_filter = 0;
    multiline = 0;
}
