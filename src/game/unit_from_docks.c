/* unit_from_docks: candidate for 1048:4b3e (233 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: unit_from_docks
 *
 * Creates a unit on the player's Europe dock from a profession code, and returns its index
 * or -1. The code goes in +0x17 of the record and picks the unit type:
 *
 *     0x14 -> type 2, and 100 in amount[5]   (the tools a Pioneer carries)
 *     0x18 -> type 3
 *     0x16 -> type 5                         (the type explore_square gives extra sight)
 *     0x15 -> type 1, or type 4 on a 1-in-(difficulty + 5) roll
 *     anything else -> type 0
 *
 * The four tests are separate `if`s over one value rather than a chain, which is what the
 * shipped bytes do -- four independent compares, no `else`. Orders are set to 1.
 *
 * The square is `(g_cur_nation - 20, g_cur_nation - 20)`, which is this build's Europe:
 * `num_cargo_vessels` walks the unit chain at exactly that square, and
 * `europe_new_wars` musters the King's Regulars there before the war. Negative
 * coordinates are off-map by construction, so each nation gets its own.
 *
 * The roll is the interesting one. `diff` is the difficulty for a live European player and
 * 1 otherwise, and the chance of type 4 instead of type 1 is 1 in (diff + 5) -- so the
 * HARDER the game the less often it happens. Whatever type 4 is, it is the better of the
 * pair and the difficulty makes it rarer, which is the shape of a bonus rather than a
 * penalty.
 *
 * `0x15` is the same value `CheatMenuColonist` stamps into +0x17 by default for unit types 7 and
 * 9, and the same one `europe_new_wars` stamps into the King's Regulars. So +0x17 is the
 * profession field (12 files call it `spec`, 26 call it `route` -- see
 * docs/findings/struct-layouts.md; this function is evidence for `spec` at least here),
 * `0x15` is a veteran-soldier profession and `0x1c`, `CheatMenuColonist`'s default for a plain
 * land unit, is the ordinary colonist.
 *
 * The callers are `europe_recruit_any`, the `LOSTCITY0` rumour, `TUTORIAL5` and
 * `foreign_country` twice, and the last reads the code out of `+2` of a record reached
 * through the pointer at DGROUP:0x4cc4 -- a list whose entries carry a profession
 * byte, which is what the Europe dock queue would look like.
 *
 * NAME: descriptive. The Macintosh build has `unit_from_docks(short)` with this arity, and
 * it is a plausible pairing, but this function puts a unit ON the dock square rather than
 * taking one off it, and two of its four callers are a rumour and a tutorial rather than
 * the dock queue. Recorded as a candidate, not adopted.
 *
 * RENAMED 2026-09-26 from europe_new_unit: the Mac build's unit_from_docks(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller europe_new_turn; caller europe_recruit; caller europe_recruit_any; caller foreign_country), the runner-up gets 3, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "units.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

int unit_from_docks(int code)
{
    int diff;
    register int type;
    register int u;

    type = 0;
    if (code == 0x14)
        type = 2;
    if (code == 0x18)
        type = 3;
    if (code == 0x16)
        type = 5;
    if (code == 0x15) {
        type = 1;
        diff = (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN)
               ? g_game.difficulty : 1;
        if (rand_range(0, diff + 4) == 0)
            type = 4;
    }
    u = create_unit(type, g_cur_nation, g_cur_nation - 20, g_cur_nation - 20);
    if (u < 0)
        goto done;
    g_units[u].orders = ORDERS_SENTRY;
    g_units[u].spec = (unsigned char)code;
    if (type == 2)
        g_units[u].hold.amount[5] = 0x64;
done:
    return u;
}
