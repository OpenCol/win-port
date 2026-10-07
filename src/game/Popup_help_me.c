/* Popup_help_me: candidate for 1050:0a88 (36 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Whether bit 7 of the byte at +0x86 of the record is set.
 *
 * RENAMED 2026-09-26 from flag_86_high_bit: whether the popup's second action was used: bit 0x80 of the
 * flags at +0x86, which button_click sets for a button of code 1, after callers enable
 * the action with bit 0x40. europe_purchase, europe_recruit, choose_freedom and
 * ColonyWin_colony_hammer_change answer it by opening the Colonopedia on the selected item and
 * re-running the popup; colony_select_profession uses it to flip pages and run_text_dialog
 * reports it as g_dialog_help_used. Medium confidence: the help reading is the majority use.
 *
 * RENAMED 2026-09-26 from popup_help_requested: the Mac build's Popup::help_me(void): this only, whether the popup's help action was used (bit 0x80 of the flags at +0x86, set by button_click for button code 1). It sits in the Popup module (1050) and the width matches.
 */

#include "colonization.h"

typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

int Popup_help_me(Popup *popup)
{
    return (popup->flags & 0x80) != 0;
}
