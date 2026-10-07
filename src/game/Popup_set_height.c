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

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_height(Popup *popup, int height)
{
    popup->min_height = height;
}
