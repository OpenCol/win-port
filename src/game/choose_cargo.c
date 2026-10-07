/* choose_cargo: candidate for 1028:cc32 (184 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: choose_cargo
 * name-source: sibling-build
 *
 * Pops up the sixteen goods under a caller-supplied title and returns which one was picked.
 * The entries are `strings(g_goods_name_word[i])` -- the same SEG20:0xd86e table
 * four other matched files already name -- and sixteen is the number of goods.
 *
 * NAME: the Macintosh build's `choose_cargo(char*)`. One `char*` argument, and this takes
 * one pointer and nothing else. Arity and role together, the §96 argument.
 *
 * The return is the popup's event MINUS TWO, in two steps the source spells separately:
 * `r = event - 1` and then `return r - 1`. Item ids run from 1 and the caller wants a
 * zero-based goods index, so one of the two is the id-to-index shift and the other is the
 * popup's own off-by-one; the shipped `DEC AX` and `LEA AX,[DI-1]` are what keep them apart.
 *
 * `r` is initialised to 0 in its declaration even though every path overwrites it -- the
 * `XOR DI,DI` is there in the shipped prologue and does not appear without it.
 */

#include "colonization.h"

#include "goods.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern void Popup_item(Popup *popup, char *s, int id);
extern char *strings(int n);

int choose_cargo(char *title)
{
    int r = 0;
    register int i;
    Popup p(0x800);

    show_plain_dialog_1028_e43b(&p, title, 0);
    p.flags |= 1;
    for (i = 0; i < GOODS_COUNT; i++)
        Popup_item(&p, strings(g_goods_name_word[i]), i + 1);
    r = popup_run_thunk_1028_e46b(&p) - 1;
    return r - 1;
}

