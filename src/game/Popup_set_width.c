/* Popup_set_width: candidate for 1050:0cee (25 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Stores its second argument in the popup's word at +0x358 and does nothing else.
 *
 * WHAT THE CALLERS SAY IT IS. Four of them: `(&popup, 0x1e0)`, `(l.popup, 300)`, `(p, 0x1b8)`
 * and `(p, rect_width_1050_830c(r))` -- 480, 300, 440, and a rectangle's width. A width is the
 * only thing all four are, and the last one names itself.
 *
 * IT NEEDED A RECONSTRUCTION RATHER THAN A RENAME. `Popup_start` reaches it through
 * the four-byte `PUSH CS / CALL NEAR`, so `check_call_forms` resolves the callee through the
 * symbol map -- and the map takes a name from a function's own reconstruction, which this had
 * none of. Renaming the stub alone was tried and reverted; this is the repair that sticks.
 *
 * RENAMED 2026-09-26 from popup_set_width: the Mac build's Popup::set_width(int): this plus the width. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_width(Popup *popup, int width)
{
    popup->width = width;
}
