/* Popup_hilite: candidate for 1050:0e54 (71 bytes).  Verdict: RELOCATABLE.
 * symbol: Popup_hilite
 * name-source: sibling-build
 *
 * Sets or clears flag 2 on the list item with the given key, doing nothing when no item has that key.
 *
 * RENAMED 2026-09-26 from item_flag2_set: sets or clears bit 2 of a popup item's flags, the preselected item:
 * colony_select_profession marks the colonist's present specialty, ColonyWin_colony_hammer_change the colony's
 * current build, or "(More)" when nothing else is preselected -- each beside Popup_set_active.
 *
 * RENAMED 2026-09-26 from popup_item_set_current: the Mac build's Popup::hilite(int, int): this plus an item key and on/off, marking the preselected item. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern PopupItem * Popup_find_item(void *popup, int key);
void Popup_hilite(Popup *popup, int key, int on)
{
    PopupItem *item;
    item = Popup_find_item(popup, key);
    if (item != 0) {
        if (on)
            item->flags |= 2;
        else
            item->flags &= ~2;
    }
}
