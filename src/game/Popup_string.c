/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 40, 56 of the imported file. */
/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
/* Popup_string: candidate for 1050:0966 (290 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends one line to a text list. The list is a singly-linked chain from +0x376, walked to
 * its end -- there is no tail pointer -- and both the ten-byte node and the copy of the string
 * come out of the object's OWN arena at +0x392, so nothing here calls the allocator.
 *
 * A LEADING CARET IS MARKUP. `^` sets bit 1 of the flags at +0x86 and is eaten; `^^` sets bit
 * 0 instead and eats both. Anything else is stored verbatim, caret and all, because the test
 * only ever looks at the first character. The flags are on the LIST, not the line, so the last
 * line to carry a caret wins.
 *
 * The comparisons go through `CBW` and then `CMP AX,0x5E`, so the source writes `(int)*s` --
 * rule 51's explicit cast, twice, where the plain comparison would be one `CMP BYTE`.
 *
 * `s++` on a pointer is `INC WORD [BP+0xA]`: the offset only, never the segment.
 *
 * `prev` is initialised in its DECLARATION -- the `MOV DWORD [BP-6],0` is the function's first
 * instruction, ahead of the load of the head pointer.
 *
 * RENAMED 2026-09-26 from text_line_append: the Mac build's Popup::string(char*): this plus a line, appending it to the popup's text list (+0x376) out of its own arena (+0x392). It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

PopupLine *Popup_string(Popup *popup, char *s)
{
    PopupLine *prev = 0;
    PopupLine *n;

    n = popup->lines;
    while (n != 0) {
        prev = n;
        n = n->next;
    }
    n = (PopupLine *)heap_get((MemBlock *)(&popup->arena), 10);
    if (prev != 0)
        prev->next = n;
    else
        popup->lines = n;
    n->next = 0;
    n->flags = 0;
    if ((int)*s == 0x5e) {
        s++;
        if ((int)*s == 0x5e) {
            s++;
            popup->flags |= 1;
        } else {
            popup->flags |= 2;
        }
    }
    n->text = (char *)heap_get((MemBlock *)(&popup->arena), STRLEN(s) + 1);
    STRCPY(n->text, s);
    popup->n_lines++;
    return n;
}
