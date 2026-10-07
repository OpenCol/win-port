/* Popup_button: candidate for 1050:167f (135 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a string to the control's list, up to THREE and then silently not at all: the
 * count at +0x82 gates the whole body, `strlen + 1` bytes come out of the control's own
 * arena at +0x392, the pointer goes into the -pointer array at +0x3b8, and the count goes
 * up.
 *
 * The arena is the one `Popup_set_title` uses for the single text at +0x362, so a control
 * carries one allocator and two things allocated from it.
 *
 * The count is read back from memory THREE times -- once for the guard, once to index the
 * store, once to index the copy -- and incremented from memory at the end. Nothing is kept
 * in a register across the two calls, which is what `-Od` gives.
 *
 * The array index is `count * 4` with a `SHL`, and the store is two words rather than one
 * 32-bit `MOV`, because the value arrives from `heap_get` already split in DX:AX.
 *
 * RENAMED 2026-09-26 from ctl_add_string: the Mac build's Popup::button(char*): this plus a caption, appended to the popup's button captions, at most three. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

#include "records.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

extern void *heap_get(void *arena, int n);

void Popup_button(Popup *popup, char *s)
{
    if (popup->n_extra < 3) {
        popup->extra_labels[popup->n_extra] = (char *)heap_get(&popup->arena, STRLEN(s) + 1);
        STRCPY(popup->extra_labels[popup->n_extra], s);
        popup->n_extra++;
    }
}
