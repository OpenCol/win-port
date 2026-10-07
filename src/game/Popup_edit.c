/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 45, 52, 59 of the imported file. */
/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
/* Popup_edit: candidate for 1050:142a (597 bytes).  Verdict: RELOCATABLE.
 * size-source: 597, not the census 590. The trailing 7 bytes are the dead second epilogue.
 * name-source: sibling-build
 *
 * Appends a TEXT-ENTRY item to a Popup, the third item kind beside `Popup_item` and
 * `Popup_sprite`. The 0x14-byte item goes on the end of the list at +0x386. Its label
 * is copied with a space appended, and the popup's label column at +0x352 is widened to the
 * label's extent plus one more space. The field length is `maxlen` capped at 255, and the item's
 * width at +2 is that many "W"s -- the widest glyph -- in the popup's font. The popup's overall
 * width at +0x35e is widened to the label column plus the field. A 0x100-byte buffer at +0xc takes
 * the initial text, truncated to the field length and terminated, or starts empty when there is
 * none; the item is numbered from the count at +0x7a.
 *
 * `(cap = 0xff, &cap)` is the cap passed by address to `min_ptr_1050_979f`: the store sits inside
 * the argument, which is why the shipped code pushes SS after the `LEA` rather than before
 * (rule 61).
 *
 * RENAMED 2026-09-26 from popup_edit_item: the Mac build's Popup::edit(char*, char*, int): this plus a label, a text buffer and a maximum length, in that order -- adding an edit field. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetTextLength_1050_8391(Font *font, const char *s);

PopupEdit *Popup_edit(Popup *popup, char *label, char *text, int maxlen)
{
    PopupEdit *tail;
    PopupEdit *it;
    int w;
    int lw;
    int cap;

    tail = 0;
    it = popup->edit_items;
    while (it != 0) {
        tail = it;
        it = it->next;
    }
    it = (PopupEdit *)heap_get((MemBlock *)(&popup->arena), 0x14);
    if (tail != 0)
        tail->next = it;
    else
        popup->edit_items = it;
    it->next = 0;
    it->f0 = 0;
    it->label = (char *)heap_get((MemBlock *)(&popup->arena), STRLEN(label) + 2);
    STRCPY(it->label, label);
    STRCAT(it->label, " ");
    lw = MSGetTextLength_1050_8391(popup->font, it->label) + MSGetTextLength_1050_8391(popup->font, " ");
    popup->label_w = *max_ptr_1050_97d1(&popup->label_w, &lw);
    maxlen = *min_ptr_1050_979f((cap = 0xff, &cap), &maxlen);
    it->maxlen = maxlen;
    it->buf = (char *)heap_get((MemBlock *)(&popup->arena), 0x100);
    it->width = MSGetTextLength_1050_8391(popup->font, "W") * maxlen;
    w = popup->label_w + it->width;
    popup->edit_w = *max_ptr_1050_97d1(&popup->edit_w, &w);
    if (text == 0) {
        it->buf[0] = 0;
    } else {
        STRNCPY(it->buf, text, maxlen);
        it->buf[maxlen] = 0;
    }
    it->index = popup->n_edits++;
    return it;
}
