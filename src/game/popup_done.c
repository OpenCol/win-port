/* PORT: this file's own declarations of EventWin_EndModal removed: the header declares it from the definition (W1 step 3). */
/* popup_done: candidate for 1050:2c32 (48 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets bit 0x400 in the current popup's flags and ends its owner window's modal session.
 *
 * RENAMED 2026-09-26 from ctl_flag_and_refresh: the old name misread the body: nothing is refreshed. It sets bit 0x400 in the current popup's flags and ends the popup's modal session (EventWin_EndModal on its owner's window); a list selection, a dialog menu command and the Enter key in a list reach it.
 *
 * RENAMED 2026-09-27 from popup_end_modal: BEYOND THE PROPOSALS: the CODE 19 popup-control block maps one to one onto 1050:2c32..3319 (see chains). Mac popup_done(void), CODE 19 0x34b6, 56 bytes, calls EventWin::EndModal and is called by button_click, key_click and radio_double_click. 2c32 (void), 48 bytes, calls EventWin_EndModal and is called by dialog_menu_command, list_key and ctl_select_item_index (= those three, below).
 */

#include "colonization.h"

#include "classes.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;   /* DGROUP:0x2500 */

void popup_done(void)
{
    g_cur_dialog->flags |= 0x400;
    EventWin_EndModal((EventWin *)((char *)g_cur_dialog->win + 0x26));
}
