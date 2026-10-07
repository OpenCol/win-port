/* ctl_next_1060_abe3: candidate for 1060:abe3 (29 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The NEXT control on the window's list: the pointer at +0x12. A one-line accessor,
 * 20-odd bytes of prologue and epilogue around a single move.
 *
 * `DialogWin_AddControl` appends through +0x12 and `find_control`, `list_find_by_id` and
 * `DialogWin_RemoveControl` all walk with this. It is `DlgControl`'s `next` field.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

DlgControl *ctl_next_1060_abe3(DlgControl *ctl)
{
    return ctl->next;
}
