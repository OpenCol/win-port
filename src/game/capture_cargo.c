/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 78 of the imported file. */
/* capture_cargo: candidate for 1020:2166 (655 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: capture_cargo
 *
 * Which of a beaten unit's cargo slots the winner takes. Returns the slot, or -1 for
 * nothing, and leaves a flag at DGROUP:0x4bea saying whether the whole hold was taken.
 *
 * THE HUMAN IS ASKED AND THE AI IS NOT, and the two halves are visibly different code.
 * A human winner (`nation < 4 && control == 0`, the idiom §120 settled) gets `PICKACARGO`
 * listing each slot as amount-space-goods-name, with a cancel item; the AI scores every
 * slot as `value * amount` from the 16-per-nation byte table at SEG30:0x10, sorts, and
 * takes the last -- the most valuable.
 *
 * `sort_16x16(count, idx, value)` is the same helper `europe_recruit_any`
 * uses, with the same parallel-array shape: keys in one array, the thing to sort by in the
 * other, and the answer read off the end.
 *
 * Nothing happens at all when the winner already has room for everything: the guard is
 * `cargo_check(winner, 1, 1) >= count`, so a hold that can take the lot skips the choice.
 *
 * Two spellings the bytes fixed, both from §130's lessons an hour earlier.
 *
 * The Popup is declared LAST even though it is allocated at the top of the frame, because
 * `pick = -1` stores its value BEFORE the constructor runs. Every scalar and array first,
 * object last.
 *
 * And `idx[slot] = (unsigned char)slot`, not `= slot`: the shipped loads the index as a
 * byte and promotes it with `MOV AH,0` before the word store -- the same cast the recruit
 * screen needed for the same kind of parallel array.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern void Popup_item(Popup *popup, char *s, int id);
extern char *strings(int n);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);

extern void space_string(char *buf);
extern void sort_16x16(int n, int *payload, int *key);
extern int cargo_check(int u, int need, int last_ship);

int capture_cargo(int loser, int winner)
{
    int count;
    int pick = -1;
    int nation;
    int slot;
    int type;
    int amount;
    int value[6];
    int idx[6];
    char buf[0x50];
    Popup p(0x800);

    nation = NATION(g_units[winner]);
    count = g_units[loser].cargo_count;
    g_cargo_captured = 0;
    if (count == 0)
        goto out;
    g_cargo_captured = 1;
    pick = 0;
    if (cargo_check(winner, 1, 1) >= count)
        goto out;
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        show_plain_dialog_1020_a8d3(&p, (void*)("PICKACARGO"), 0);
        Popup_item(&p, strings(g_misc_words[MISC_NOTHING]), 0x63);
        for (slot = 0; slot < count; slot++) {
            type = get_cargo(loser, slot);
            amount = get_cargo_amount(loser, slot);
            buf[0] = 0;
            say_number(buf, amount);
            space_string(buf);
            say_string(buf, g_goods_name_word[type]);
            Popup_item(&p, buf, slot);
        }
        pick = popup_run_thunk_1020_a903(&p);
        if (pick == 0x63) {
            pick = -1;
            goto out;
        }
        if (pick > 0) {
            g_cargo_captured = 0;
            pick--;
        } else {
            pick = 0;
        }
        goto out;
    }
    for (slot = 0; slot < count; slot++) {
        type = get_cargo(loser, slot);
        amount = get_cargo_amount(loser, slot);
        idx[slot] = (unsigned char)slot;
        value[slot] = g_goods_value[nation][type] * amount;
    }
    sort_16x16(count, idx, value);
    pick = idx[count - 1];
out:
    return pick;
}

