/* ctl_kind_1060_ac19: candidate for 1060:ac19 (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * The control's KIND: the word at +0.
 *
 * `focus_walk_siblings` and `dialog_mnemonic` compare it against 2, 3, 4, 6 and 7, and
 * `MSCloseWindow` against 3 -- the same enumeration the `dlg_control_init` copies write
 * there: 1 static region, 2 checkbox, 3 image button or radio, 4 edit box, 6 button,
 * 7 listbox, 8 scroll bar. `focus_walk_siblings` gives the focus to 6 (if +0x1a is set),
 * 2 (if +0x20 is set), 4 and 7, which are exactly the focusable kinds.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int ctl_kind_1060_ac19(DlgControl *ctl)
{
    return ctl->kind;
}
