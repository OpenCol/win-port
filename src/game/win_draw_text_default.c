/* win_draw_text_default: candidate for 1048:e326 (54 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * size-source: 54, not the census 50. The trailing 4 bytes are a SECOND epilogue, and
 * they are there because the body ends in an explicit `return;` -- Borland emits an
 * epilogue for the return statement AND one for the end of the function (rule 44).
 *
 * The window's text drawer with the global dialog context g_popup_game_txt (the resource-file string "GAME") and the window's flag word +0x86.
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

/* same object: 4-byte `push cs; call ` (rule 47) */

void win_draw_text_default(Popup *popup, char *name, int sel, int unused)
{
    text_dialog_plain_1048_e7f3(popup, g_popup_game_txt, name, sel, popup->flags);
    return;
}
