/* Popup_edit_y_size: candidate for 1050:0b5b (54 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The line height of the popup's font (pointer at +0x6e) plus 8.
 *
 * RENAMED 2026-09-26 from ctl_text_width_plus_8: the Mac build's Popup::edit_y_size(void): this only -- the font's line height plus 8, the step between edit rows (Popup_compute_size multiplies it by n_edits; Popup_make_controls places row k at ey + k times it). It measures a HEIGHT: +0x6e is the popup's font, which MSGetLineHeight_1050_834b reads, not a text. The width matches.
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetLineHeight_1050_834b(Font *font);
int Popup_edit_y_size(Popup *popup)
{
    int r;

    r = MSGetLineHeight_1050_834b(popup->font) + 8;
    return r;
}
