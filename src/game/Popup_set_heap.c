/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 27, 28 of the imported file. */
/* Popup_set_heap: candidate for 1050:00fa (55 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Empties a Popup's string arena by freeing it and allocating it again: `heap_destroy` on
 * the block at +0x392, then `heap_create(block, 9, size)`.
 *
 * +0x392 is the arena the popup's own text comes out of -- `Popup_button` puts each string
 * there and `Popup_string` says its lines come "out of the object's OWN arena at +0x392".
 * The one caller, `Popup_init`, calls this right after `Popup_close` with the
 * popup's word at +0x39c as the size, when a popup that is already up is being pointed at
 * new contents.
 *
 * Both calls are , into segment 1040, so both callees are extern.
 *
 * RENAMED 2026-09-26 from popup_arena_reset: the Mac build's Popup::set_heap(int): this plus a size, freeing the popup's string arena and allocating it again at that size. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern void heap_destroy(MemBlock *block);
extern int heap_create(MemBlock *heap, int kind, unsigned size);

void Popup_set_heap(Popup *popup, unsigned size)
{
    heap_destroy((MemBlock *)(&popup->arena));
    heap_create((MemBlock *)(&popup->arena), 9, size);
}
