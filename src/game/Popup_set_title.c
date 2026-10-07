/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 24 of the imported file. */
/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
/* Popup_set_title: candidate for 1050:0c96 (88 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Gives the control its own copy of a string: `strlen + 1` bytes out of the arena that
 * lives at +0x392 of the control itself, the returned pointer parked at +0x362, and the
 * string copied in. The allocation goes through `heap_get`, which takes the arena
 * by pointer and the size.
 *
 * The arena address is built as `(char *)c + 0x392` with the control's own segment --
 * the object owns its allocator rather than sharing a global one.
 *
 * RENAMED 2026-09-26 from ctl_copy_text: the Mac build's Popup::set_title(char*): this plus a string, copied into the popup's own arena (+0x392) and parked at +0x362 -- the title Popup_make_window gives the window. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

#include "records.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */


void Popup_set_title(Popup *popup, char *s)
{
    popup->title = (char *)heap_get((MemBlock *)(&popup->arena), STRLEN(s) + 1);
    STRCPY(popup->title, s);
}
