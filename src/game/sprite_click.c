/* sprite_click: candidate for 1050:2cef (220 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A click on row `id - 0x12c` of the current dialog's item list: walk that many links down
 * the list at +0x37a, record the item's id at +0xa8 and the item itself at +0x37e, repaint,
 * and call the list's own callback at +0x38a with the id.
 *
 * Three guards, and all three are on the OBJECT rather than on the row: no list at all, a
 * list whose first item has no successor, and a non-zero item count at +0x7c. So a single-item list is
 * not clickable, which is the second guard's whole content.
 *
 * The id is turned into a row by subtracting 0x12c IN PLACE -- the parameter itself is
 * decremented -- so the caller's command ids start at 300.
 *
 * RENAMED 2026-09-27 from list_click: BEYOND THE PROPOSALS. Mac sprite_click(short), CODE 19 0x3568, 234 bytes. Its calls are bsr RedrawSpriteSquare, DrawSpriteSquare, Popup::draw, plus CopyToScreen/ValidRect. 2cef (int id), 220 bytes, calls list_unhighlight, list_highlight, Popup_draw and copy_all_to_screen (= those, below). No direct callers; it is a handler. Layout: third in the block on both. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;                 /* DGROUP:0x2500 */
extern void RedrawSpriteSquare(void);
extern void DrawSpriteSquare(void);
extern void Popup_draw(Popup *d);

/* same object: 4-byte `push cs; call ` */

void sprite_click(int id)
{
    PopupSprite *sprite_item;
    register int i;

    if (g_cur_dialog->sprites != 0 && g_cur_dialog->sprites->next != 0 && g_cur_dialog->n_items == 0) {
        id -= 0x12c;
        RedrawSpriteSquare();
        sprite_item = g_cur_dialog->sprites;
        for (i = 0; i < id; i++)
            sprite_item = sprite_item->next;
        g_cur_dialog->result = sprite_item->id;
        g_cur_dialog->cur_sprite = sprite_item;
        DrawSpriteSquare();
        copy_all_to_screen_1050_843b(g_cur_dialog->win);
        if (g_cur_dialog->on_click != 0) {
            (*(void (*)(int))g_cur_dialog->on_click)(sprite_item->id);
            Popup_draw(g_cur_dialog);
        }
    }
}
