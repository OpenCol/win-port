/* Popup_find_item: candidate for 1050:0d41 (102 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * The first node of a list whose key field matches, or a null pointer when the list runs out.
 *
 * RENAMED 2026-09-26 from list_find_by_key: the Mac build's Popup::find_item(int): this plus a key, the item lookup every per-item setter (grey, hilite, set_check) goes through. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
PopupItem * Popup_find_item(Popup *popup, int key)
{
    PopupItem *found = 0;
    PopupItem *item = popup->items;
    while (item != 0 && found == 0) {
        if (item->id == key)
            found = item;
        item = item->next;
    }
    return found;
}
