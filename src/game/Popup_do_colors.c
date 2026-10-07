/* Popup_do_colors: candidate for 1050:1706 (81 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Installs one of the control's three state bytes (+0x94, +0x96, +0x92) as the global,
 * chosen by two flags.
 *
 * RENAMED 2026-09-26 from ctl_swap_state_byte: the Mac build's Popup::do_colors(int, int): this plus two flags, choosing which of the popup's three colour bytes (+0x92, +0x94, +0x96) becomes the text colour; Popup_draw calls it with (0, 0) for the plain one. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_do_colors(Popup *popup, int a, int b)
{
    if (a)
        TextColor(popup->f94);
    else if (b)
        TextColor(popup->hilite_color);
    else
        TextColor(popup->text_color);
}
