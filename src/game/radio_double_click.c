/* radio_double_click: candidate for 1050:2c62 (141 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks the current control's item list counting to index idx, records that item's id + 1
 * as the selection (-1 when none), and ends the popup's modal session (popup_done). The first parameter is unused.
 *
 * RENAMED 2026-09-27 from ctl_select_item_index: BEYOND THE PROPOSALS. Mac radio_double_click(short, short), CODE 19 0x34ee, 122 bytes. Its body, decoded, reads the SECOND argument (move.w $a(a6)), walks the current popup's item list at +0x380 to that index, stores -1 in +0xba or the found item's +2 field plus one, and calls popup_done. 2c62 (int unused, int idx), 141 bytes, is that code line for line (g_cur_dialog->items, result = -1 or found->id + 1, popup_end_modal). No direct callers on either side; it is installed as a handler. Layout: second in the block on both.
 */

#include "colonization.h"

#include "records.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;   /* DGROUP:0x2500 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void popup_done(void);
void radio_double_click(int unused, int idx)
{
    PopupItem *item;
    PopupItem *found = 0;
    int i;

    if (g_cur_dialog == 0)
        return;
    i = 0;
    item = g_cur_dialog->items;
    while (item != 0) {
        if (i == idx)
            found = item;
        i++;
        item = (PopupItem *)item->next;
    }
    g_cur_dialog->result = -1;
    if (found != 0)
        g_cur_dialog->result = found->id + 1;
    popup_done();
}
