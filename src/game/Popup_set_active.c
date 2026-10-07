/* Popup_set_active: candidate for 1050:0fad (43 bytes).  Verdict: RELOCATABLE.
 * symbol: Popup_set_active
 * name-source: sibling-build
 *
 * Stores what Popup_find_item finds for a in the pointer at +0x366
 * of the record.  The finder is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from store_entry_ptr: the Mac build's Popup::set_active(int): this plus an item key; it looks the item up (Popup_find_item) and stores it as the popup's selected item (+0x366), and every caller does that just before running the popup to preselect the current choice -- the colonist's job, the build item, the trade-route stop. The width matches.
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern char g_stub_s[];

extern void *Popup_find_item(Popup *popup, int key);

void Popup_set_active(Popup *popup, int key)
{
    popup->sel_item = Popup_find_item(popup, key);
}
