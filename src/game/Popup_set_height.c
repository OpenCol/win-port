/* Popup_set_height: candidate for 1050:0d07 (24 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Stores its second argument in the popup's word at +0x76. The twin of
 * `Popup_set_width` immediately before it, and `Popup_init` calls the pair
 * in one breath -- `rect_width_1050_830c(r)` to that one and `rect_height_1050_832b(r)` to this.
 *
 * THE TWO FIELDS ARE NOT ADJACENT, which is worth saying because it looks like a mistake: the
 * width goes to +0x358 and the height to +0x76. Both stores are single instructions in the
 * shipped bytes and neither is in doubt.
 *
 * RENAMED 2026-09-26 from popup_set_height: the Mac build's Popup::set_height(int): this plus the height. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_height(Popup *popup, int height)
{
    popup->min_height = height;
}
