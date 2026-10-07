/* Popup_ctor: candidate for 1050:03c2 (179 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup::Popup(int)
 *
 * The constructor of Popup (0x3dc bytes; a GRPort at +8 and two Sprites at +0x2e/+0x4e):
 * the reset routine Popup_clear twice around a 9-byte-per-entry memblock of n entries at
 * +0x392, then the port initialised 40x40. The reset routine is defined in this file
 * because the shipped calls to it are the compiler's own `push cs / call `: it zeroes
 * the popup's bookkeeping, sets the -1 sentinels, takes the four default COLOUR bytes (text, highlight, and two more -- not margins) from
 * DGROUP:0x24f8..0x24fb and the fixed spacing (2, 2, 5, 4) and sets the popup's column count at +0x84 to 1.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern void Popup_set_font(Popup *popup, Font *owner);
extern int heap_create(MemBlock *heap, int kind, unsigned size);

extern "C" extern "C" void Popup_clear(Popup *popup);

Popup::Popup(int n)
{
    Popup_clear(this);
    heap_create(&arena, 9, n);
    Popup_clear(this);
    GRPort_Init_wh(&port, 0x28, 0x28);
}
