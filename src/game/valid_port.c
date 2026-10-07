/* valid_port: candidate for 1028:bd95 (543 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 543, not the census 534. The trailing 9 bytes are the dead second epilogue.
 *
 * Can unit `u` get to colony `colony`? Both are optional: 999 means "no colony" and a
 * negative index means "no unit", and with neither supplied the answer is yes. Map plane 2's
 * low nibble is the region id, and the whole function is a comparison of two of them.
 *
 * A SHIP (type 0xd..0x12) is matched by WATER. Off the map it counts as the high seas, two
 * columns short of the map width on row 1; on the map it uses its own square's region if no
 * neighbour is ocean, and otherwise the ocean square `water_access` left in
 * DGROUP:0x4bc8/0x4bca -- the lowest-numbered neighbouring water. The colony must likewise
 * touch water, and the two water regions must agree. Anything else is matched by LAND: the
 * unit's square against the colony's.
 *
 * A colony not owned by the nation on turn (SEG20:0x7796) answers no before anything else is looked at.
 *
 * `ask_port` is the caller this was named from: it counts how many colonies the
 * ship can actually reach before offering the list.
 *
 * THE REGISTER ALLOCATOR IS THE WHOLE PUZZLE HERE, and it settles a question rule 53 left
 * open. The shipped puts the FIRST parameter in DI and the second in SI, with both locals on
 * the stack. One function-wide `int region` takes DI instead -- even with `register` written
 * on both parameters, which is the case rule 53 says cannot happen. Giving each branch its
 * own block-scoped `region` shortens the live ranges and the parameter wins. So the ordering
 * is by live range, and `register` on a parameter does not lift it over a local that is live
 * across the whole body.
 *
 * `(unsigned char)region` on both comparisons: `continent_at` returns `unsigned char`,
 * so the assignment widens with `MOV AH,0` into a word, but the shipped compares `CMP
 * AL,[BP-6]` -- one byte. Without the cast Borland promotes and the compare is a word wider.
 *
 * The inner `if (colony != 999)` has an EMPTY `else`: its body ends with an `EB 00` into the
 * `JMP` that skips the land branch, which is one jump more than the same `if` without one
 * (rule 54). Both early exits are the two-jump form and so are `goto`s, and the written-out
 * `return r` leaves a second, dead epilogue behind it (rules 12 and 44).
 *
 * RENAMED 2026-09-26 from unit_reaches_colony: the Mac build's valid_port(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller ask_port; callee continent_at; callee on_map; callee set_colony; callee water_access), the runner-up gets 3, and the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern void set_colony(int colony);

extern unsigned char continent_at(int x, int y);
extern int water_access(int x, int y);

int valid_port(int colony, int unit)
{
    int r;

    r = 0;
    if (colony != 999) {
        set_colony(colony);
        if (g_cur_colony_rec->nation != ((unsigned char)g_game.turn_nation))
            goto out;
    }
    if (unit >= 0) {
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) {
            int region;
            if (on_map(g_units[unit].x, g_units[unit].y) == 0)
                region = continent_at(g_map_size.w - 2, 1);
            else if (water_access(g_units[unit].x, g_units[unit].y) == 0)
                region = continent_at(g_units[unit].x, g_units[unit].y);
            else
                region = continent_at(g_ocean_x, g_ocean_y);
            if (colony != 999) {
                if (water_access(g_cur_colony_rec->x,
                                                  g_cur_colony_rec->y) == 0)
                    goto out;
                if (continent_at(g_ocean_x, g_ocean_y) != (unsigned char)region)
                    goto out;
            } else {
            }
        } else {
            int region = continent_at(g_units[unit].x, g_units[unit].y);
            if (continent_at(g_cur_colony_rec->x, g_cur_colony_rec->y) != (unsigned char)region)
                goto out;
        }
    }
    r = 1;
out:
    return r;
}
