/* ctl_next_1070_a09a: candidate for 1070:a09a (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ctl_next_1070_a09a
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The NEXT control on the window's list: the pointer at +0x12. A one-line accessor,
 * 20-odd bytes of prologue and epilogue around a single move.
 *
 * `DialogWin_AddControl` appends through +0x12; `list_find_by_id` walks with this, and
 * `find_control` and `DialogWin_RemoveControl` with the segment-1060 copy `ctl_next_1060_abe3`. It is `DlgControl`'s `next` field.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void *ctl_next_1070_a09a(DlgControl *ctl)
{
    return ctl->next;
}
