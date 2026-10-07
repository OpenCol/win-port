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
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetTextLength_1050_8391(Font *font, const char *s);
int Popup_button_x_size(Popup *ctl, char *s)
{
    return MSGetTextLength_1050_8391(ctl->font, s) + (((ctl->gap_x * 2) + 2) << 1);
}
