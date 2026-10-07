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
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
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
