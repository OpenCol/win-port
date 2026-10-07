/* key_click: candidate for 1050:2dcb (357 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The keyboard half of `sprite_click`: 0xa8 and 0xc0 step BACK through the dialog's
 * item list, 0xa2 and 0xc1 step forward, and 0xd2 clears the selection outright. Two codes per
 * direction because the arrow keys and the keypad both arrive.
 *
 * The list is doubly linked -- `next` at +0x14 and `prev` at +0x18 -- and only the forward step
 * WRAPS: running off the end puts the selection back on the head, while stepping back off the
 * front just follows whatever `prev` holds. The two directions are not symmetric.
 *
 * The same three guards as the click handler, and all three are on the OBJECT: no list, a list
 * whose first item has no successor, and a non-zero item count at +0x7c (a popup with menu items lays its sprites out differently). A single-item list takes no
 * keys at all. The 0xd2 arm is ahead of them, so the selection can always be cleared.
 *
 * `g_cur_dialog->sprites != 0` is one `CMP DWORD` while `items->next != 0` is `MOV AX / OR AX` -- the
 * same test on the same kind of pointer, compiled two ways because one is reached through
 * `LES` and the other is not. `sprite_click` shows the same pair.
 *
 * RENAMED 2026-09-27 from list_key: CONST PROPOSAL; the name is free (no record or file carries key_click in any case). Mac key_click(short), CODE 19 0x3652, 394 bytes. Its calls are popup_done, CopyToScreen/ValidRect x2, and bsr RedrawSpriteSquare x2 and DrawSpriteSquare x2. 2dcb (register int key), 357 bytes, calls popup_end_modal, copy_all_to_screen x2, list_unhighlight x2 and list_highlight x2. No direct callers; it is a handler. Layout: fourth in the block on both, after sprite_click / list_click. Constants: 5 shared incl. rare 0xa2, overlap 1.0. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;                 /* DGROUP:0x2500 */
extern void RedrawSpriteSquare(void);
extern void DrawSpriteSquare(void);

/* same object: 4-byte `push cs; call ` */
extern void popup_done(void);

void key_click(register int key)
{
    PopupSprite *sprite_item;

    if (key == 0xd2) {
        g_cur_dialog->result = -1;
        popup_done();
    } else if (g_cur_dialog->sprites != 0 && g_cur_dialog->sprites->next != 0 && g_cur_dialog->n_items == 0) {
        if (key == 0xa8 || key == 0xc0) {
            RedrawSpriteSquare();
            sprite_item = g_cur_dialog->cur_sprite->prev;
            g_cur_dialog->result = sprite_item->id;
            g_cur_dialog->cur_sprite = sprite_item;
            DrawSpriteSquare();
            copy_all_to_screen_1050_843b(g_cur_dialog->win);
        } else if (key == 0xa2 || key == 0xc1) {
            RedrawSpriteSquare();
            if (g_cur_dialog->cur_sprite->next == 0) {
                g_cur_dialog->result = g_cur_dialog->sprites->id;
                g_cur_dialog->cur_sprite = g_cur_dialog->sprites;
            } else {
                g_cur_dialog->result = g_cur_dialog->cur_sprite->next->id;
                g_cur_dialog->cur_sprite = g_cur_dialog->cur_sprite->next;
            }
            DrawSpriteSquare();
            copy_all_to_screen_1050_843b(g_cur_dialog->win);
        }
    }
}
