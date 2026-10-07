/* raise_taxes: candidate for 1048:6002 (1400 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: raise_taxes
 *
 * A TAX CHANGE, and the colonists' answer to a raise. `key` is the announcement's text key and
 * `delta` the change; `europe_new_taxes` is one of five callers.
 *
 * THE RATE (the signed byte +1 of the market record at DGROUP:0x4cc4): a cut is capped so the rate
 * does not go below 0, and the rate is capped at 75, with `delta` reduced by whatever was cut off.
 * Nothing more happens when the change comes to 0 or the nation is not a human-played European
 * (index 4 or more, or its controller byte SEG20:0x7841 set).
 *
 * THE GOODS the colonists would dump: for each goods not already boycotted (the mask at +0x20), the
 * colony of this nation with the most of it, among colonies with flag 0x40 set. Each goods' weight is
 * |+0xbc[goods]| / 100, quartered for horses and muskets and halved for food and tools. The weights of
 * the goods found are summed. A second pass then takes the stocked goods in order and subtracts each
 * weight until the sum reaches 0. Nothing random is involved, so it always lands on the LAST stocked
 * goods with a non-zero weight (the first stocked goods when every weight is 0). Boycotted goods never
 * have stock here, so the later boycott test on the chosen goods can never be true.
 *
 * PLAIN ANNOUNCEMENT, via `ask_text_resource`, when no goods was found, the change is a cut, or the goods
 * is boycotted: a cut plays `new_mood(2)` and a raise the king's music.
 *
 * THE CHOICE otherwise: the announcement comes with the two TAXOPTIONS lines, the second naming
 * "<colony> <goods>". Choosing the second (2) is the tea party. The raise is taken back, up to 100 of the
 * goods leave the colony and are added to its long at +0xc2, the goods is added to the boycott mask, and
 * TEAPARTY is shown with its music.
 *
 * Spellings the bytes insist on:
 *  - both sides of the cut's cap compare as bytes;
 *  - the cap at 75 casts to int (rule 51), and both rate reductions are written `tax = tax - n`;
 *  - the colony's nation compares as `(char)` against the low byte of the player index;
 *  - the shared exit is `return;`, which restores the exception word through DX (rule 41).
 *
 * RENAMED 2026-09-26 from apply_tax_change: the Mac build's raise_taxes(char*, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller colony_plan; caller new_turn_bookkeeping; callee item; callee popm; callee popup_say), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "goods.h"
#include "sounds.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern int g_popup_tribe;                     /* DGROUP:0x2518, declared */

extern GameState g_game;   /* SEG20:0x7782 */

extern void Popup_item(Popup *popup, char *s, int id);

extern void space_string(char *buf);

extern void expand_text_template(char *src, char *dst);
extern int text_open(void *ctx, char *name);
extern char *text_get(void);
extern void popup_say(int slot, int v);
extern int popm(char *msg, int n);
extern void sound_play(int n);
extern void new_mood(int mood);

extern "C" void set_text_arg_i_1048_db6a(int slot, int v);
extern "C" void set_name_slot_1048_de36(int slot, char *s);
extern "C" void set_text_arg_char_1048_de8f(int i, char v);

void raise_taxes(char *key, int delta)
{
    Popup p(0x800);
    int excess;
    int goods;
    int i;
    int col;
    int answer;
    int amount;
    long total;
    int colony_of[16];
    int stock[16];
    int value[16];
    char buf[0x50];
    register int colony;
    register int mask;

    if (delta < 0 && (char)g_cur_nation_rec->tax < (char)(delta > 0 ? delta : ~delta + 1))
        delta = 0 - g_cur_nation_rec->tax;
    g_cur_nation_rec->tax += (char)delta;
    if ((int)g_cur_nation_rec->tax > 75) {
        excess = g_cur_nation_rec->tax - 75;
        g_cur_nation_rec->tax = g_cur_nation_rec->tax - excess;
        delta -= excess;
    }
    if (delta == 0)
        goto out;
    if (g_cur_nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_nation].control != CONTROL_HUMAN)
        goto out;
    total = 0;
    for (i = 0; i < 16; i++) {
        colony_of[i] = -1;
        stock[i] = 0;
        value[i] = (int)(LABS(g_cur_nation_rec->trade_tons[i]) / 100);
    }
    value[GOODS_MUSKETS] >>= 2;
    value[GOODS_HORSES] >>= 2;
    value[GOODS_TOOLS] >>= 1;
    value[GOODS_FOOD] >>= 1;
    mask = g_cur_nation_rec->boycott;
    for (colony = 0; colony < g_game.colony_count; colony++) {
        if ((char)g_colonies[colony].nation != (char)g_cur_nation)
            continue;
        if (!(g_colonies[colony].status & 0x40))
            continue;
        for (i = 0; i < 16; i++) {
            if (mask & (1 << i))
                continue;
            if (g_colonies[colony].stock[i] > stock[i]) {
                stock[i] = g_colonies[colony].stock[i];
                colony_of[i] = colony;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (mask & (1 << i))
            continue;
        if (stock[i] != 0)
            total += value[i];
    }
    goods = -1;
    for (i = 0; i < 16; i++) {
        if (stock[i] == 0)
            continue;
        total -= value[i];
        if (total <= 0) {
            goods = i;
            break;
        }
    }
    set_text_arg_i_1048_db6a(0, delta > 0 ? delta : ~delta + 1);
    set_text_arg_char_1048_de8f(1, g_cur_nation_rec->tax);
    if (goods < 0 || delta < 0 || (mask & (1 << goods))) {
        if (delta < 0)
            new_mood(2);
        else
            sound_play(SOUND_MEETKING);
        g_popup_tribe = 8;
        ask_text_resource(key);
        goto out;
    }
    col = colony_of[goods];
    STRCPY(buf, g_colonies[col].name);
    space_string(buf);
    say_string(buf, g_goods_name_word[goods]);
    set_name_slot_1048_de36(3, buf);
    g_popup_tribe = 8;
    show_plain_dialog_1048_dde3(&p, key, 0);
    text_open(g_game_txt, "TAXOPTIONS");
    text_get();
    Popup_item(&p, g_text_buf, 0);
    text_get();
    expand_text_template(g_text_buf, buf);
    Popup_item(&p, buf, 1);
    sound_play(SOUND_MEETKING);
    answer = popup_run_thunk_1048_de13(&p);
    if (answer != 2)
        goto out;
    g_cur_nation_rec->tax = g_cur_nation_rec->tax - delta;
    amount = g_colonies[col].stock[goods];
    amount = min_int_1048_db44(amount, 100);
    g_colonies[col].stock[goods] -= amount;
    g_colonies[col].bells += amount;
    set_text_arg_i_1048_db6a(0, amount);
    popup_say(0, g_goods_name_word[goods]);
    set_name_slot_1048_de36(1, g_colonies[col].name);
    popup_say(2, g_nation_name_word[g_cur_nation]);
    sound_play(SOUND_MUSPARTY);
    popm("TEAPARTY", 1);
    g_cur_nation_rec->boycott |= 1 << goods;
out:
    return;
}

