/* Popup_button_y_size: candidate for 1050:0c28 (51 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The line height of the popup's font (+0x6e, through MSGetLineHeight_1050_834b) plus (its pad word at +0xa0 plus 2) << 1; the
 * shift by one is emitted as ADD DX,DX, and `* 2` would be distributed into pad_y*2 + 4.
 *
 * RENAMED 2026-09-26 from ctl_text_width_padded: the Mac build's Popup::button_y_size(void): this only -- the font's line height plus twice (pad + 2), the height of the button row (Popup_compute_size adds it to the popup's height; Popup_make_controls places the buttons at by + it - 3). A HEIGHT, as edit_y_size is. The width matches.
 */

#include "colonization.h"

#include "classes.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetLineHeight_1050_834b(Font *font);
int Popup_button_y_size(Popup *popup)
{
    return MSGetLineHeight_1050_834b(popup->font) + ((popup->pad_y + 2) << 1);
}
