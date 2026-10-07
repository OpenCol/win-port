/* Popup_button_x_size: candidate for 1050:0c5b (59 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 59, not the census 55. The trailing 4 bytes are the dead second epilogue.
 *
 * The control's text measured through its own font record, plus four times its pad word
 * plus four. The sibling of `Popup_button_y_size` 51 bytes above it, and the two differ
 * in every part: that one measures with `MSGetLineHeight_1050_834b` off the text pointer at +0x6e and
 * pads from the word at +0xa0 as `(pad + 2) << 1`; this one passes the SAME +0x6e slot as a
 * `Font *` to `MSGetTextLength_1050_8391` beside the string, and pads from
 * +0xa6 as `((pad * 2) + 2) << 1`.
 *
 * THE SECOND ARGUMENT IS ONE `char *`, not two ints: the call site at 1050:22fa pushes a
 * pointer out of `g_popup_label[]` at SEG31:0x98, and `MSGetTextLength` at the end of the
 * chain hands it to `GetTextExtentPoint` (§119).
 *
 * The doubling is emitted as `ADD DX,DX` twice around an `ADD DX,2`, so the expression is
 * built as written rather than folded to `pad * 4 + 4`.
 *
 * RENAMED 2026-09-26 from ctl_font_extent_padded: the Mac build's Popup::button_x_size(char*): this plus a caption, measured in the popup's font plus padding; Popup_compute_size sizes the buttons and labels with it. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetTextLength_1050_8391(Font *font, const char *s);
int Popup_button_x_size(Popup *ctl, char *s)
{
    return MSGetTextLength_1050_8391(ctl->font, s) + (((ctl->gap_x * 2) + 2) << 1);
}
