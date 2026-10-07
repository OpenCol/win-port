/* try_to_purchase: candidate for 1028:3402 (314 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Buys one unit on the Europe dock: look up the kind's price and type in the six-byte table at
 * SEG20:0xd83c (type at +1, price at +4), refuse if the treasury cannot cover it, create the unit at the Europe square,
 * copy two bytes of the nation record into it, mark it, and take the money. Returns 1 only if
 * all of that happened -- every refusal is a `goto` to the single exit, which is why the
 * result is a local set to 0 first and to 1 at the very end.
 *
 * Europe is `(g_game.turn_nation - 20, g_game.turn_nation - 20)`, the same off-board square
 * `num_cargo_vessels` walks and `europe_new_wars` musters at.
 *
 * `g_units[u].orders` is `(type >= 0xd && type <= 0x12) ? 0 : 1` -- the `&&` form with ZERO
 * first. The equivalent `(type < 0xd || type > 0x12) ? 1 : 0` inverts both jumps and swaps
 * the two `MOV AL`s, which is the whole difference between them in the bytes.
 *
 * TWO UNUSED PARAMETERS, one here and one below. This function's second argument is never
 * read -- its callers pass 0x64 -- and `purchase_available`, which it calls, takes the
 * recruit kind and ignores it, returning 1 for everything. Neither is visible in the function
 * that has it (§119); both come from the call sites.
 *
 * RENAMED 2026-09-26 from recruit_unit: the Mac build's try_to_purchase(short, short), bound by call-sequence alignment (5.0 aligned gaps in foreign_country leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.87x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"
                                  /* 6 bytes at SEG20:0xd83c -- the base `setup_purchase`
                                     writes, one byte BEFORE the 0xd83d the readers used */

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` */
extern int purchase_available(int kind);

int try_to_purchase(int kind, int unused1)
{
    int ok = 0;
    int price;
    register int u;

    if (purchase_available(kind) == 0)
        goto out;
    price = g_recruit[kind].price;
    if ((long)price > g_cur_nation_rec->gold)
        goto out;
    u = create_unit(g_recruit[kind].type, g_game.turn_nation,
                              g_game.turn_nation - 20, g_game.turn_nation - 20);
    if (u < 0)
        goto out;
    g_units[u].dest_x = g_cur_nation_rec->start_x;
    g_units[u].dest_y = g_cur_nation_rec->start_y;
    g_units[u].orders = (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) ? 0 : 1;
    g_cur_nation_rec->gold -= price;
    ok = 1;
out:
    return ok;
}
