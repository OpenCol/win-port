/* Popup_dtor: candidate for 1050:07ac (173 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup::~Popup()
 *
 * The destructor of Popup: Popup_close, the memblock at +0x392 freed, Popup_clear, and
 * win_enable_1050_82e1 on the pointer at DGROUP:0x24fc; the Sprite and GRPort members are the
 * compiler's.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, Sprite, EventWin, DialogWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void Popup_clear(Popup *popup);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void Popup_close(Popup *popup);
extern void heap_destroy(MemBlock *block);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_enable_1050_82e1(void *w);
   /* EventWin : Screen, EventTable, flattened for C: 0x8c bytes */
   /* DialogWin : EventWin, flattened for C */

Popup::~Popup()
{
    Popup_close(this);
    heap_destroy(&arena);
    Popup_clear(this);
    win_enable_1050_82e1((DialogWin *)g_popup_owner);
}
