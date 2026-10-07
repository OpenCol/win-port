/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of ColChild_BackspaceMessage, copy_rect_to_screen removed: the header declares them from the definition (W1 step 3). */
/* EuropeWin_AttemptCargoFromEurope: candidate for 1048:1cbd (1428 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_AttemptCargoFromEurope
 * name-source: sibling-build
 * size-source: 1428, not the census 1419: a dead second epilogue and padding before 1048:2251.
 *
 * BUYING GOODS IN EUROPE ONTO SHIP `u`. Returns 0 when the purchase went through (or was refused
 * outright by `allowed == 0`), and 1 otherwise. The name reflects only TUTORIAL18, the shortfall
 * hint. The market drags, the dock, and the trade-route step (`perform_orders_route`,
 * `ask == 0`) all buy through it.
 *
 * REFUSALS, each a status-line message on the Europe window:
 *   - during the tutorial with no ship in port: "<nation word 5> <goods>!";
 *   - no hold space for the goods: "<nation word 4> <goods>!".
 * The amount is the hold space, at most 100. With `ask` the player is asked (HOWMUCH4, with the
 * space and the price), and the answer is clamped to 0..space; cancelling or 0 buys nothing.
 *
 * NOT ENOUGH GOLD for price x amount: "<goods> (<cost>) <nation word 8>". When not asking (the
 * trade route), TUTORIAL18 explains the shortfall with the price and the treasury.
 *
 * THE PURCHASE (`cargo_from_europe`) returns the gold spent. The status line reads
 * "<amount> <goods> <word 0> <cost>". The receipt panel is redrawn at (0x140, 0x2d) with "<ddd8>
 * <amount> <goods> <ddda> <price><dddc>", and below it "<ddde>: <cost> gold". The market then moves
 * for that goods (`europe_adjust_prices(0, goods)`), and the tutorial refreshes the dock.
 *
 * A SHIPPED SLIP: the final `copy_rect_to_screen` is handed a local RECT that nothing ever fills.
 *
 * The goods number is a register COPY of the parameter (rule 73), and the nation index at DGROUP:0x4cc8
 * is declared in this module.
 *
 * RENAMED 2026-09-26 from tutorial_cargo_purchase: Buys goods in Europe onto ship u from the Europe window: checks hold room (cargo_holds_free), optionally asks HOWMUCH4, checks the treasury against price x amount, calls cargo_from_europe, writes the 'bought N goods at P' receipt and moves the market (europe_adjust_prices). The market drags, the dock and the trade-route step all buy through it. The tutorial parts (no-ship refusal, the TUTORIAL18 shortfall hint when not asking, the tutorial dock refresh) are minor branches; the old name names a side effect instead of the purchase that is the function's job.
 *
 * RENAMED 2026-09-27 from europe_buy_cargo: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). BEYOND THE THREE LEADS. It reverses the witness_renames.json rejection, which rested only on 'the Mac twin has no symbol (anon)'. That twin, anon_33_3d0, does have one: CODE 33 LINK 0x3d0 .. RTS 0x8a2 is closed by the long-form name 'AttemptCargoFromEurope__9EuropeWinFssss'. Callees: the Mac set is cargo_holds_free, check_gold, cargo_from_europe, europe_ask_price, europe_adjust_prices, popup_ask_number, minimax, popup_say, EuropeClearBig, say_bucks and the message helpers, and it is 1cbd's set item for item (cargo_holds_free, check_gold, cargo_from_europe, europe_ask_price x4, europe_adjust_prices, popup_ask_number, minimax, popup_say x3, EuropeWin_EuropeClearBig, say_bucks, ...). Callers: Mac EuropeMouseUnits, EuropeMouseMarket (114a), EuropeMouseShipsCargo (13e4), perform_orders_route; Windows europe_cargo_click, europe_market_click, europe_ships_click, perform_orders_route. Arity: this+(short x4) against (EuropeWin *w, u, goods_arg, allowed, ask). Layout: first of the three Attempt* on both sides. Strings: HOWMUCH4/TUTORIAL18 are shared (from the earlier rejection). Size: 1234 Mac against 1428. No collision.
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"

extern int g_ships_in_port;               /* DGROUP:0x1fca, declared : ships in port */

extern void set_europe(int n);
extern int cargo_holds_free(int u, int goods, int *room);
extern void popup_say(int slot, int word);
extern int europe_ask_price(int g);
extern int popup_ask_number(char *ctx, char *name, int n);

extern long check_gold(int nation);
extern int cargo_from_europe(int u, int goods, int want);

extern void space_string(char *buf);

extern void colon_string(char *buf);

extern void europe_adjust_prices(int reset, int only);
extern "C" void europe_change_mode(void);   /* PORT: C linkage, as defined (W2) */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_CancelMessage(EuropeWin *child, int a);
extern "C" void EuropeWin_EuropeMessage(EuropeWin *win, int i);
extern "C" void ColChild_AddToMessage(EuropeWin *child, int word);
extern "C" void ColChild_AddStringToMessage(EuropeWin *message, char *s);
extern "C" void EuropeWin_EuropeActivateMessage(EuropeWin *win, int kind, long ms);

extern "C" void set_text_arg_i_1048_db6a(int slot, int v);

extern "C" void EuropeWin_EuropeClearBig(EuropeWin *w);
extern "C" void EuropeWin_EuropeShowBig(EuropeWin *w);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
int EuropeWin_AttemptCargoFromEurope(EuropeWin *w, int u, int goods_arg, int allowed, int ask)
{
    register int goods = goods_arg;
    register int cost;
    int r;
    int room;
    int requested;
    int ty;
    RECT rect;
    char buf[0x50];

    r = 1;
    set_europe(w->nation);
    if (g_europe_screen_open != 0 && g_ships_in_port == 0) {
        ColChild_CancelMessage(w, 1);
        EuropeWin_EuropeMessage(w, 5);
        ColChild_AddToMessage(w, g_goods_name_word[goods]);
        ColChild_BackspaceMessage(w);
        ColChild_AddStringToMessage(w, "!");
        EuropeWin_EuropeActivateMessage(w, 3, 0x78L);
        goto out;
    }
    if (allowed == 0) {
        r = 0;
        goto out;
    }
    if (!cargo_holds_free(u, goods, &room)) {
        ColChild_CancelMessage(w, 1);
        EuropeWin_EuropeMessage(w, 4);
        ColChild_AddToMessage(w, g_goods_name_word[goods]);
        ColChild_BackspaceMessage(w);
        ColChild_AddStringToMessage(w, "!");
        EuropeWin_EuropeActivateMessage(w, 3, 0x78L);
        goto out;
    }
    room = min_int_1048_db44(room, 100);
    if (ask != 0) {
        popup_say(0, g_goods_name_word[goods]);
        popup_say(1, g_unit_types[g_units[u].type].name);
        set_text_arg_i_1048_db6a(0, room);
        set_text_arg_i_1048_db6a(1, europe_ask_price(goods));
        if (popup_ask_number(g_game_txt, "HOWMUCH4", room))
            goto out;
        requested = minimax((int)g_text_args[0], 0, room);
        room = min_int_1048_db44(requested, room);
        if (room <= 0)
            goto out;
    }
    cost = europe_ask_price(goods) * room;
    if ((long)cost > check_gold(g_cur_nation)) {
        ColChild_CancelMessage(w, 1);
        ColChild_AddToMessage(w, g_goods_name_word[goods]);
        ColChild_AddStringToMessage(w, "(");
        ColChild_BackspaceMessage(w);
        ColChild_AddNumberToMessage(w, cost);
        ColChild_BackspaceMessage(w);
        ColChild_AddStringToMessage(w, ")");
        EuropeWin_EuropeMessage(w, 8);
        EuropeWin_EuropeActivateMessage(w, 3, 0x78L);
        if (ask == 0) {
            popup_say(0, g_goods_name_word[goods]);
            set_text_arg_i_1048_db6a(0, europe_ask_price(goods));
            set_text_arg_l_1048_db8e(1, g_cur_nation_rec->gold);
            ask_text_resource("TUTORIAL18");
        }
        goto out;
    }
    cost = cargo_from_europe(u, goods, room);
    ColChild_CancelMessage(w, 1);
    ColChild_AddNumberToMessage(w, min_int_1048_db44(100, room));
    ColChild_AddToMessage(w, g_goods_name_word[goods]);
    EuropeWin_EuropeMessage(w, 0);
    ColChild_AddNumberToMessage(w, cost);
    EuropeWin_EuropeActivateMessage(w, 1, 0x78L);
    EuropeWin_EuropeClearBig(w);
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_BOUGHT]);
    space_string(buf);
    say_number(buf, min_int_1048_db44(100, room));
    space_string(buf);
    say_string(buf, g_goods_name_word[goods]);
    space_string(buf);
    say_string(buf, g_misc_words[MISC_AT]);
    space_string(buf);
    say_number(buf, europe_ask_price(goods));
    say_string(buf, g_misc_words[MISC_TON]);
    TextColor(g_text_colour);
    ty = 0x2d;
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, w->rect.left + 0x140, w->rect.top + ty, 0x10);
    ty += 0x1e;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_PRICE]);
    colon_string(buf);
    say_bucks(buf, (long)cost);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, w->rect.left + 0x140, w->rect.top + ty, 0x10);
    EuropeWin_EuropeShowBig(w);
    copy_rect_to_screen(w, &rect);
    europe_adjust_prices(0, goods);
    if (g_europe_screen_open != 0)
        europe_change_mode();
    r = 0;
out:
    return r;
}
