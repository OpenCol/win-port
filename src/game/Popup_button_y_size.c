/* Popup_button_y_size: candidate for 1050:0c28 (51 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The line height of the popup's font (+0x6e, through MSGetLineHeight_1050_834b) plus (its pad word at +0xa0 plus 2) << 1; the
 * shift by one is emitted as ADD DX,DX, and `* 2` would be distributed into pad_y*2 + 4.
 *
 * RENAMED 2026-09-26 from ctl_text_width_padded: the Mac build's Popup::button_y_size(void): this only -- the font's line height plus twice (pad + 2), the height of the button row (Popup_compute_size adds it to the popup's height; Popup_make_controls places the buttons at by + it - 3). A HEIGHT, as edit_y_size is. The width matches.
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetLineHeight_1050_834b(Font *font);
int Popup_button_y_size(Popup *popup)
{
    return MSGetLineHeight_1050_834b(popup->font) + ((popup->pad_y + 2) << 1);
}
