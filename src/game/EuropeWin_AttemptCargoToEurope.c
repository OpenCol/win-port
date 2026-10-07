/* EuropeWin_AttemptCargoToEurope: candidate for 1048:2251 (1832 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_AttemptCargoToEurope
 * name-source: sibling-build
 * size-source: 1832, not the census 1823. The trailing 9 bytes are the dead second epilogue.
 *
 * SELLING A SHIP'S CARGO IN EUROPE, and the printed receipt that goes with it. Five callers reach it:
 * the market click (`EuropeWin_EuropeMouseMarket`), the ship menu (`europe_click_ship`),
 * `perform_orders_unload`, `perform_orders_route` and the dock click `EuropeWin_EuropeMouseUnits`.
 * It answers 0 when the sale happened and 1 when it did not.
 *
 * The ship must actually carry the goods (`find_cargo`); if not, and the window
 * is showing (DGROUP:0xb4), the message line says so and the function stops. With `ask` set, HOWMUCH5
 * asks for the quantity, defaulting to 100; the answer comes back in SEG31:0x2d4 and is clamped
 * to 0..100.
 *
 * THE SALE: `cargo_to_europe` returns the gross; the tax is `gross * tax% / 100` in 32-bit
 * arithmetic, the nation is credited the net, and the market's running totals at +0x22 and +0x26 take
 * the tax and the net. The same figures then go out twice -- once as the window's message line, and
 * once as a four-line receipt drawn straight into the window: the headline, the gross, the tax as a
 * negative, a double rule, and the net.
 *
 * THE RULE UNDER THE TAX is drawn as four lines, and they do not line up: the two lower ones (y+2,
 * y+3) in the shadow colour run 0xf5..0x195, the two upper (y, y+1) in the text colour run
 * 0xf0..0x190 and 0xf0..0x195.
 * The last one is five pixels longer than the one above it -- a typo in the shipped source, visible
 * as a single pixel of overhang on the receipt.
 *
 * WHAT THE BYTES SETTLED: three declarations, each found by one instruction. `set_europe`
 * takes an int in this module (the nation is pushed as a word, not loaded as a byte first), the
 * market's tax byte is cast straight to `long` before the multiply (`MOVSX EDX` rather than a trip
 * through 16 bits), and DGROUP:0x337e is an int here though other modules read it as a byte. `y` and
 * `kind` are the two `register` locals, SI and DI in declaration order (rule 53), and only the five
 * NOP-padded calls are extern (rule 47).
 *
 * RENAMED 2026-09-27 from europe_sell_cargo: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). BEYOND THE THREE LEADS. It reverses the earlier rejection 'the Mac twin is anonymous'. anon_33_8d0 is CODE 33 LINK 0x8d0 .. RTS 0xe5a, closed by the long-form name 'AttemptCargoToEurope__9EuropeWinFsss'. Callers match five for five: Mac EuropeMouseUnits, EuropeMouseMarket (114a), europe_click_ship, perform_orders_route, perform_orders_unload; Windows EuropeWin_EuropeMouseUnits, europe_market_click, europe_click_ship, perform_orders_route, perform_orders_unload. Callees: find_cargo, cargo_to_europe, add_gold, europe_bid_price, europe_adjust_prices, popup_ask_number, minimax, popup_say, DrawLine, EuropeClearBig and say_bucks on both sides. Arity: this+(short x3) against (ColChild *w, unit, goods, ask). Strings: HOWMUCH5 is shared. Layout: second of three on both sides. Size: 1418 Mac against 1832. No collision.
 */

#include "colonization.h"

#include "misc.h"
#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"

extern void set_europe(int n);
extern int find_cargo(int u, int goods);
extern void popup_say(int slot, int word);
extern int europe_bid_price(int g);
extern int popup_ask_number(char *ctx, char *name, int n);

extern int cargo_to_europe(int unit, int slot, int keep);
extern void add_gold(int n, long amount);

extern void space_string(char *buf);
extern void colon_string(char *buf);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y,
                                     int size);
extern void GRPort_DrawLine(GRPort *w, short x1, short y1, short x2, short y2);
extern void europe_adjust_prices(int reset, int only);
extern void europe_change_mode(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_CancelMessage(ColChild *child, int a);
extern "C" void EuropeWin_EuropeMessage(EuropeWin *win, int n);
extern "C" void ColChild_AddToMessage(ColChild *child, int idx);

extern "C" void ColChild_BackspaceMessage(ColChild *w);
extern "C" void ColChild_AddStringToMessage(ColChild *message, char *s);
extern "C" void EuropeWin_EuropeActivateMessage(EuropeWin *m, int kind, long ms);
extern "C" void set_text_arg_i_1048_db6a(int i, int v);

extern "C" void EuropeWin_EuropeClearBig(EuropeWin *w);
extern "C" void EuropeWin_EuropeShowBig(EuropeWin *w);

int EuropeWin_AttemptCargoToEurope(EuropeWin *w, int unit, int goods, int ask)
{
    int result;
    int slot;
    int gross;
    int tax;
    int net;
    int requested;
    int amount;
    char buf[0x50];
    register int y;
    register int kind;

    kind = goods;
    result = 1;
    amount = 100;
    set_europe(w->nation);
    slot = find_cargo(unit, kind);
    if (slot < 0) {
        if (g_europe_screen_open != 0) {
            ColChild_CancelMessage(w, 1);
            EuropeWin_EuropeMessage(w, 9);
            ColChild_AddToMessage(w, g_goods_name_word[kind]);
            EuropeWin_EuropeMessage(w, 0xa);
            EuropeWin_EuropeActivateMessage(w, 3, 0x78L);
        }
        goto out;
    }
    if (ask != 0) {
        popup_say(0, g_goods_name_word[kind]);
        popup_say(1, g_unit_types[g_units[unit].type].name);
        popup_say(2, g_nation_name_word[g_cur_nation]);
        set_text_arg_i_1048_db6a(0, g_cargo_amount);
        set_text_arg_i_1048_db6a(1, europe_bid_price(kind));
        if (popup_ask_number(g_game_txt, "HOWMUCH5", amount))
            goto out;
        requested = minimax((int)g_text_args[0], 0, amount);
        amount = min_int_1048_db44(requested, amount);
        if (amount <= 0)
            goto out;
    }
    gross = cargo_to_europe(unit, slot, amount);
    tax = (long)gross * (long)g_cur_nation_rec->tax / 100;
    net = gross - tax;
    add_gold(g_cur_nation, (long)net);
    g_cur_nation_rec->royal_gold += (long)tax;
    g_cur_nation_rec->net_take += (long)net;
    ColChild_CancelMessage(w, 1);
    ColChild_AddNumberToMessage(w, g_cargo_amount);
    ColChild_AddToMessage(w, g_goods_name_word[kind]);
    EuropeWin_EuropeMessage(w, 1);
    ColChild_AddNumberToMessage(w, gross);
    ColChild_BackspaceMessage(w);
    ColChild_AddStringToMessage(w, ".");
    ColChild_AddNumberToMessage(w, (int)g_cur_nation_rec->tax);
    ColChild_BackspaceMessage(w);
    EuropeWin_EuropeMessage(w, 0x11);
    ColChild_AddNumberToMessage(w, tax);
    ColChild_BackspaceMessage(w);
    EuropeWin_EuropeMessage(w, 0x12);
    ColChild_AddNumberToMessage(w, net);
    EuropeWin_EuropeActivateMessage(w, 1, 0x78L);
    EuropeWin_EuropeClearBig(w);
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_SOLD]);
    space_string(buf);
    say_number(buf, g_cargo_amount);
    space_string(buf);
    say_string(buf, g_goods_name_word[kind]);
    space_string(buf);
    say_string(buf, g_misc_words[MISC_AT]);
    space_string(buf);
    say_number(buf, europe_bid_price(kind));
    say_string(buf, g_misc_words[MISC_TON]);
    y = 0x14;
    TextColor(g_text_colour);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, 0x140, w->rect.top + y, 0x10);
    y += 0x1e;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_PRICE]);
    colon_string(buf);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, 0x12c, w->rect.top + y, 0x10);
    buf[0] = 0;
    say_bucks(buf, (long)gross);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, 0x190, w->rect.top + y, 0x10);
    y += 0x1e;
    buf[0] = 0;
    say_number(buf, (int)g_cur_nation_rec->tax);
    say_string(buf, g_misc_words[MISC_TAX]);
    colon_string(buf);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, 0x12c, w->rect.top + y, 0x10);
    buf[0] = 0;
    say_bucks(buf, (long)-tax);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, 0x190, w->rect.top + y, 0x10);
    y += 0xf;
    TextColor(g_draw_color);
    GRPort_DrawLine((GRPort *)w, 0xf5, w->rect.top + y + 2, 0x195, w->rect.top + y + 2);
    GRPort_DrawLine((GRPort *)w, 0xf5, w->rect.top + y + 3, 0x195, w->rect.top + y + 3);
    TextColor(g_text_colour);
    GRPort_DrawLine((GRPort *)w, 0xf0, w->rect.top + y, 0x190, w->rect.top + y);
    GRPort_DrawLine((GRPort *)w, 0xf0, w->rect.top + y + 1, 0x195, w->rect.top + y + 1);
    y += 0xf;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_NET]);
    colon_string(buf);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, 0x12c, w->rect.top + y, 0x10);
    buf[0] = 0;
    say_bucks(buf, (long)net);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, buf, 0x190, w->rect.top + y, 0x10);
    y += 0x1e;
    EuropeWin_EuropeShowBig(w);
    europe_adjust_prices(0, kind);
    if (g_europe_screen_open != 0)
        europe_change_mode();
    result = 0;
out:
    return result;
}
