/* popup_setup_plain_1008_ce18: candidate for 1008:ce18 (39 bytes).  Verdict: RELOCATABLE.
 *
 * Popup_init with three zero longs and a zero word after the pointer.
 *
 * TRANSFERRED: byte-identical to `popup_setup_plain_1048_e387` (1048:e387), one of 2 copies of this
 * function in the image. Same source, this segment's copy.
 *
 * RENAMED 2026-09-26 from text_call_0859_1008_ce18: Popup_init with every option zero -- three null longs and a
 * zero word after the popup.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "classes.h"
extern void Popup_init(Popup *popup, GameWin *win, GRPort *port, RECT *r, int kind);

void popup_setup_plain_1008_ce18(void *p)
{
    Popup_init((Popup *)p, 0L, 0L, 0L, 0);
}
