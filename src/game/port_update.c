/* port_update: candidate for 1050:3319 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Repaints the popup at DGROUP:0x2500 through `Popup_draw`, and does nothing when there is
 * no popup. `Popup_draw` (1050:4829) is the popup's full repaint, a direct call, so this is
 * the no-argument wrapper that supplies the current popup.
 *
 * It is itself a HANDLER: `Popup_make_window` pushes its address as a pointer into
 * `set_relayout_handler_1050_857d`, a slot setter, so it is installed rather than called. That is why it
 * takes no arguments and reads a global -- the slot's signature has no room for one.
 *
 * The name is a leftover from when Popup_draw was read as a slot invoker; the bytes
 * establish a direct call to the popup repaint.
 *
 * `Popup_draw` arrives as TLINK's 5-byte NOP/PUSH CS/CALL NEAR, so it stays `extern`;
 * defined in-file the call comes out four bytes wide and the function is one byte short.
 *
 * RENAMED 2026-09-27 from call_slot910_global: it calls Popup_draw on g_cur_dialog (the current popup) when there is one -- no handler slot is involved; the set_relayout_handler pair installs it as the relayout callback.
 *
 * RENAMED 2026-09-27 from redraw_current_popup: BEYOND THE PROPOSALS. Mac port_update(void), CODE 19 0x3bde, 48 bytes. Its only call is bsr Popup::draw. 3319 (void), 38 bytes, calls only Popup_draw. Neither has a direct caller. Layout: after button_click, directly before Popup::make_window (1050:3364, already bound), on both. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;   /* DGROUP:0x2500 */
extern void Popup_draw(Popup *w);

void port_update(void)
{
    if (g_cur_dialog != 0)
        Popup_draw(g_cur_dialog);
}
