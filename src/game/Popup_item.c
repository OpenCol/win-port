/* PORT: a record holding pointers is allocated by its sizeof (the game's size on i386, larger on 64-bit Windows) (W9). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 31, 43 of the imported file. */
/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
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
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

extern int MSGetTextLength_1050_8391(Font *font, const char *s);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int Popup_item_x_size(Popup *popup);

PopupItem *Popup_item(Popup *popup, char *s, int id)
{
    PopupItem *tail = 0;
    PopupItem *it;
    i16 w;   /* PORT: i16, max_ptr_1050_97d1 takes its address */

    it = popup->items;
    while (it != 0) {
        tail = it;
        it = it->next;
    }
    it = (PopupItem *)heap_get((MemBlock *)(&popup->arena), sizeof(PopupItem));
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
    it->text = (char *)heap_get((MemBlock *)(&popup->arena), STRLEN(s) + 2);
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
