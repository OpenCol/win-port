/* Popup_set_font: candidate for 1050:0b91 (47 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: Popup_set_font
 *
 * Stores the popup's font at +0x6e and recomputes the row height at +0x9e from it with Popup_item_y_size.
 *
 * RENAMED 2026-09-26 from ctl_set_value: the Mac build's Popup::set_font(Font*): this plus a font, stored at +0x6e -- every caller passes one (g_popup_font, g_popup_font_small, &g_info_font_large) -- and the row height at +0x9e recomputed from it with Popup_item_y_size. The old name read the font as a 'value'. The width matches.
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int Popup_item_y_size(Popup *popup);
void Popup_set_font(Popup *popup, Font *new_font)
{
    popup->font = new_font;
    popup->row_h = Popup_item_y_size(popup);
}
