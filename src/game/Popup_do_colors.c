/* Popup_do_colors: candidate for 1050:1706 (81 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Installs one of the control's three state bytes (+0x94, +0x96, +0x92) as the global,
 * chosen by two flags.
 *
 * RENAMED 2026-09-26 from ctl_swap_state_byte: the Mac build's Popup::do_colors(int, int): this plus two flags, choosing which of the popup's three colour bytes (+0x92, +0x94, +0x96) becomes the text colour; Popup_draw calls it with (0, 0) for the plain one. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_do_colors(Popup *popup, int a, int b)
{
    if (a)
        TextColor(popup->f94);
    else if (b)
        TextColor(popup->hilite_color);
    else
        TextColor(popup->text_color);
}
