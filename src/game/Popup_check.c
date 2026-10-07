/* Popup_check: candidate for 1050:13d7 (83 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 83, not the census 76. The trailing 7 bytes are the dead second epilogue.
 *
 * `Popup_item` with two extra effects: bits 0 and 2 go on in the popup's flag word at +0x86
 * before the item is added, and when `mark` is set bit 2 goes on in the new item's own flag
 * word. It returns the item, as `Popup_item` does.
 *
 * The `|= 5` is one instruction on memory -- `OR WORD [es:bx+0x86],5` -- so both bits are set
 * together rather than in two statements.
 *
 * Item and Popup layouts are `Popup_item`'s, 427 bytes above (1050:122c), unchanged.
 *
 * RENAMED 2026-09-26 from popup_add_item_marked: the Mac build's Popup::check(char*, int, int): this plus a caption, an id and the initial mark, in that order -- adding a check item. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern PopupItem *Popup_item(Popup *popup, char *s, int id);
PopupItem *Popup_check(Popup *popup, char *s, int id, int mark)
{
    PopupItem *it;

    popup->flags |= 5;
    it = Popup_item(popup, s, id);
    if (mark != 0)
        it->flags |= 4;
    return it;
}
