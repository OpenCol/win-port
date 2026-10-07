/* Popup_item: candidate for 1050:122c (427 bytes).  Verdict: RELOCATABLE.
 *
 * Appends an item to a Popup's doubly linked list (head +0x366/+0x36e, tail +0x372, 18-byte
 * items from the popup's memblock at +0x392): the text is copied into the block, an empty
 * text sets flag bit 0, the id stored, the popup's width (+0x356) widened to the item's text
 * width plus the margins, and the item numbered from the count at +0x7c.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern void *heap_get(void *block, int n);

extern int MSGetTextLength_1050_8391(Font *font, const char *s);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int Popup_item_x_size(Popup *popup);

PopupItem *Popup_item(Popup *popup, char *s, int id)
{
    PopupItem *tail = 0;
    PopupItem *it;
    int w;

    it = popup->items;
    while (it != 0) {
        tail = it;
        it = it->next;
    }
    it = (PopupItem *)heap_get(&popup->arena, 0x12);
    if (tail != 0) {
        tail->next = it;
        it->prev = tail;
    } else {
        popup->items = it;
        popup->sel_item = it;
        it->prev = 0;
    }
    popup->last_item = it;
    it->next = 0;
    it->flags = 0;
    it->text = (char *)heap_get(&popup->arena, STRLEN(s) + 2);
    STRCPY(it->text, s);
    if (*s == 0)
        it->flags |= 1;
    it->id = id;
    w = MSGetTextLength_1050_8391(popup->font, it->text) + Popup_item_x_size(popup);
    w += popup->item_margin;
    popup->item_w = *max_ptr_1050_97d1(&w, &popup->item_w);
    it->index = popup->n_items++;
    return it;
}
