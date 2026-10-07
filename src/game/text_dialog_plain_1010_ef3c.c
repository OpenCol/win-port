/* text_dialog_plain_1010_ef3c: candidate for 1010:ef3c (62 bytes).  Verdict: RELOCATABLE.
 *
 * Popup_start with its four long arguments zero: the plain
 * TEXT dialog call used by the colony module.
 *
 * NOT TWELVE SCALARS. The callee is
 * `(void *p, void *a, void *b, int c, void *z, void *d, void *e,
 * void *f, int g)` -- the shape `run_text_dialog` and `popup_ask_nocancel`
 * declare, the second of which pushes a real pointer into the `z` slot as two words and so
 * proves it. A forwarder that passes everything through can never show a split (§119).
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "w16types.h"
#include "classes.h"
extern int Popup_start(Popup *p, void *ctx, char *name, int sel, char *dflt, GameWin *win, GRPort *port, RECT *rect, int kind);

int text_dialog_plain_1010_ef3c(void *p, void *file, void *name, int sel, int kind)
{
    return Popup_start((Popup *)p, file, (char *)name, sel, 0L, 0L, 0L, 0L, kind);
}
