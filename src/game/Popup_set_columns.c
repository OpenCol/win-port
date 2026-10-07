/* Popup_set_columns: candidate for 1050:0bc0 (25 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Stores its second argument in the popup's word at +0x84. One of the family of one-line popup
 * setters around 1050:0bc0..0d07 -- `Popup_set_width` and
 * `Popup_set_height` are the two whose field the callers name.
 *
 * RENAMED 2026-09-26 from set_w_84: stores the popup's item-grid column count at +0x84 -- what the
 * COLUMNS= directive sets (Popup_start); Popup_compute_size divides the item count by it for rows.
 *
 * RENAMED 2026-09-26 from popup_set_columns: the Mac build's Popup::set_columns(int): this plus the column count. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_columns(Popup *popup, int columns)
{
    popup->cols = columns;
}
