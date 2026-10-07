/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 99, 113 of the imported file. */
/* foreign_cargo: candidate for 1020:9126 (1400 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: foreign_cargo
 *
 * A UNIT TRADES AT A FOREIGN COLONY, the current colony at DGROUP:0x4b9e, for `1020:969e`. Returns 1
 * on every path.
 *
 * REFUSALS, in order:
 *  - the unit's nation is not a human-played European: nothing is shown;
 *  - bit 0x40 of the pair flags for (nation, owner) is clear: TRADEATWAR;
 *  - the nation lacks Jan de Witt: TRADEMERCANTILISM, with the owner's leader in slot 0;
 *  - no cargo: TRADENOCARGO.
 *
 * With more than one hold filled, TRADEWHICH lists the holds (with a Cancel) and the choice picks the
 * one to trade.
 *
 * THE PRICE: `value` is the owner's goods value (SEG30:0x10, 16 per nation) times the amount. The owner's
 * tax (+1 of its SEG20:0xc2f4 record) comes off to give `net`. `net` is halved when pair flag 2 is set
 * before independence (bit 0 of SEG20:0x7784). A further cut is 10..12d+12 percent at random, or a fixed
 * d+5 percent when trading with the intervening power (SEG20:0x77d6) after independence. The floor is 1.
 *
 * THE BARTER OFFER: for every other goods the colony holds, up to 100 of it. Before independence food,
 * lumber, tools and muskets are never offered. After it, trading muskets or horses gets 100 from the
 * intervening power and at least 50 elsewhere, stock or not. The amount is cut until it costs no more
 * than `value` -- the GROSS value, not `net` -- and the costliest offer wins.
 *
 * With no offer, TRADENOWANT. Otherwise TRADEWITH offers the goods and the gold. Answer 1 swaps the hold
 * for the colony's goods and answer 2 sells for `net` gold; anything above 2 walks away. On either deal the
 * colony gains the unit's goods. The goods it gives in a swap are NOT taken from its stock.
 *
 * `r = 1` initialises before the Popup, which is declared last (rule 72). `net = net >> 1` is the
 * spelled-out form the bytes want.
 *
 * RENAMED 2026-09-27 from foreign_colony: String witness: TRADEATWAR, TRADEMERCANTILISM, TRADENOCARGO, TRADENOWANT, TRADEWITH, TRADEWHICH and LEADER2 are all in Mac foreign_cargo(short), with a score of 111.99 against a runner-up of 14.57. The Mac foreign_colony has no strings. Call graph: Mac foreign_cargo's only caller is foreign_city, and 9126's only caller is foreign_city (1020:969e, high). Mac foreign_city's sequence set_colony, foreign_scout, foreign_cargo, end_unit_turn matches 969e's callees set_colony, foreign_scout, 9126, end_unit_turn. The callee sets match too: get_treaty x2, has_father, great, get_cargo x2, get_cargo_amount x2, itoa/LTOA1, space_string, say_string, Popup item x2, strings, random, popup_say x3, set_cargo, set_cargo_amount and unload_cargo. Arity: (short) against foreign_colony(int u). Size is 1346 Mac against 1400 Windows, while Mac foreign_colony is 40 bytes. Why it went wrong: tools/mac_rename.py mapped the paraphrase trade_with_foreign_colony onto the nearest-sounding Mac identifier, and its arity check was unmeasured.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "gameflag.h"
#include "goods.h"
#include "fathers.h"
#include "misc.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];             /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */

extern void Popup_item(Popup *popup, char *s, int id);
extern char *strings(int n);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern int set_cargo(int u, int slot, int type);
extern void set_cargo_amount(int u, int slot, int amt);
extern int unload_cargo(int u, int slot);

extern void space_string(char *buf);
extern int get_treaty(int nation_a, int nation_b);
extern int has_father(int nation, int bit);
extern void great(int slot, char *name, int n);
extern int text_dialog_sel_b0(char *key, int nation);
extern void popup_say(int slot, int v);

int foreign_cargo(int u)
{
    int r = 1;
    int nation;
    int owner;
    int type;
    int amount;
    int value;
    int best_goods;
    int price;
    int best;
    int best_amount;
    int slot;
    int choice;
    int tax;
    int net;
    char buf[0x50];
    Popup p(0x800);
    register int goods;
    register int n;

    nation = NATION(g_units[u]);
    owner = g_cur_colony_rec->nation;
    if (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN)
        goto out;
    if ((get_treaty(nation, owner) & 0x40) == 0) {
        show_text_dialog_1020_aa02((void*)("TRADEATWAR"));
        goto out;
    }
    if (has_father(nation, FATHER_JAN_DE_WITT) == 0) {
        great(0, "LEADER2", owner);
        text_dialog_sel_b0("TRADEMERCANTILISM", owner);
        goto out;
    }
    if (g_units[u].cargo_count == 0) {
        text_dialog_sel_b0("TRADENOCARGO", owner);
        goto out;
    }
    slot = 0;
    if (g_units[u].cargo_count > 1) {
        show_plain_dialog_1020_a8d3(&p, (void*)("TRADEWHICH"), 0);
        for (goods = 0; g_units[u].cargo_count > goods; goods++) {
            type = get_cargo(u, goods);
            amount = get_cargo_amount(u, goods);
            LTOA1(amount, buf, 10);
            space_string(buf);
            say_string(buf, g_goods_name_word[type]);
            Popup_item(&p, buf, goods);
        }
        Popup_item(&p, strings(g_misc_words[MISC_NOTHING]), 0x63);
        choice = popup_run_thunk_1020_a903(&p) - 1;
        if (choice < 0 || choice == 0x63)
            goto out;
        slot = choice;
    }
    type = get_cargo(u, slot);
    amount = get_cargo_amount(u, slot);
    value = g_goods_value[owner][type] * amount;
    tax = g_nations[owner].tax * value / 100;
    net = value - tax;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 && (get_treaty(nation, owner) & 2))
        net = net >> 1;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 || g_game.intervention_nation != owner)
        net -= rand_range(10, g_game.difficulty * 12 + 12) * net / 100;
    else
        net -= (g_game.difficulty + 5) * net / 100;
    net = max_int_1020_a887(1, net);
    best = -1;
    best_goods = -1;
    best_amount = 0;
    for (goods = 0; goods < 16; goods++) {
        if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0
            && (goods == GOODS_MUSKETS || goods == GOODS_TOOLS || goods == GOODS_FOOD || goods == GOODS_LUMBER))
            continue;
        if (goods == type)
            continue;
        n = g_cur_colony_rec->stock[goods];
        n = min_int_1020_a8ad(100, n);
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
            if (type == GOODS_MUSKETS || type == GOODS_HORSES) {
                if (g_game.intervention_nation == owner)
                    n = 100;
                else
                    n = max_int_1020_a887(n, 50);
            }
        }
        price = g_goods_value[owner][goods] * n;
        while (price > value) {
            n--;
            price -= g_goods_value[owner][goods];
        }
        if (n <= 0)
            continue;
        if (price > best) {
            best = price;
            best_amount = n;
            best_goods = goods;
        }
    }
    if (best < 0) {
        popup_say(0, g_goods_name_word[type]);
        set_text_arg_i_1020_a926(0, amount);
        text_dialog_sel_b0("TRADENOWANT", owner);
        goto out;
    }
    popup_say(1, g_goods_name_word[type]);
    set_text_arg_i_1020_a926(1, amount);
    popup_say(0, g_goods_name_word[best_goods]);
    set_text_arg_i_1020_a926(0, best_amount);
    set_text_arg_i_1020_a926(2, net);
    choice = text_dialog_sel_b0("TRADEWITH", owner);
    if (choice > 2)
        goto out;
    if (choice == 1) {
        set_cargo(u, slot, best_goods);
        set_cargo_amount(u, slot, best_amount);
    } else {
        unload_cargo(u, slot);
        g_nations[nation].gold += net;
    }
    g_cur_colony_rec->stock[type] += amount;
out:
    return r;
}

