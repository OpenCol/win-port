/* Popup_set_result: candidate for 1050:102c (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a word at +0xa8 of a record, in the `set_w_<slot>_<addr>` shape the corpus already
 * has four of. One `MOV [ES:BX+0xa8],AX` through a record pointer, no return of the
 * previous value.
 *
 * The record is the Popup: this sits among the Popup_set_* setters of module 1050, and +0xa8
 * is the popup's `result`, which Popup_exec returns and key_click/button_click set. It has
 * no caller in the census.
 *
 * RENAMED 2026-09-27 from set_w_a8: the record is the Popup: the setter sits among module 1050's Popup_set_* functions, and its offset is the member every Popup declaration names (result +0xa8, on_click +0x38a, draw_sprite +0x38e). The Mac build has no such method, so the name is descriptive.
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_result(Popup *popup, int v)
{
    popup->result = v;
}
