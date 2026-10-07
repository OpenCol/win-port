/* get_defense: candidate for 1020:8017 (674 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 674, not the census 665. The trailing 9 bytes are the dead second epilogue.
 *
 * SCALES A UNIT'S COMBAT STRENGTH BY WHERE IT STANDS, and records which bonus applied in
 * `g_combat_flags[1]` for the combat report. `best_defender` uses the answer as the
 * high half of its defender score.
 *
 * A NATIVE SETTLEMENT square is worth 2, another 2 if the tribe's level is at least 2 (flag
 * 0x10), and the whole DOUBLED for a settlement with bit 2 of its flags (flag 0x20); flag 8
 * always. A COLONY square is worth 2 plus twice the fort level (flag 0x40). Otherwise the
 * TERRAIN's byte from the 16-byte row table applies (flag 0x80) -- unless the defender is a
 * European and the attacker is either a native or a human European with SEG20:0x7784 bit 0 set; a
 * native attacker then never meets it, and the human one only when either square is a colony; on genuinely open ground the
 * terrain figure goes to DGROUP:0x4be6 and `g_combat_flags[0]` bit 7 instead, so the report can
 * say it was NOT applied.
 *
 * A unit on orders 6 that is not a ship gets 2 more when the bonus is still below 5 (flag 0x2000), so it can reach 6.
 * The answer is `(bonus + 4) * strength >> 2`, so a bonus of 0 leaves the strength alone.
 *
 * TWO SPELLINGS THE BYTES INSIST ON: `g_defence_village = village = find_village(x, y)`
 * as one chained assignment -- two statements reload AX -- and an EMPTY `else` on the
 * human-attacker test, whose `EB 00` is the jump the inner test lands on. Putting the empty
 * `else` on the inner `if` instead moves that landing two bytes.
 *
 * The `!IS_HUMAN(n)` test is expanded twice, so `an >= 4` is asked again inside a branch that
 * already knows `an < 4` -- a macro, and a redundant compare in the shipped code.
 *
 * RENAMED 2026-09-26 from defence_bonus: the Mac build's get_defense(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller indian_move; callee get_combat; callee is_colony; callee is_village; callee set_colony), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "nations.h"
#include "tribes.h"
#include "control.h"
#include "settle.h"
#include "gameflag.h"
#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int is_village(int x, int y);
extern int find_village(int x, int y);
extern int is_colony(int x, int y);

extern void set_colony(int colony);

/* same object: 4-byte `push cs; call ` */
extern int get_combat(int u, int attacking);
extern int get_colony_defense(void);

int get_defense(register int u, int attacker)
{
    int str;
    int terr;
    int village;
    int tribe;
    int x;
    int y;
    int open;
    int an;
    register int bonus;

    x = g_units[u].x;
    y = g_units[u].y;
    an = NATION(g_units[attacker]);
    str = get_combat(u, 0);
    bonus = 0;
    g_terrain_defence = 0;
    if (is_village(x, y) >= 0) {
        bonus = 2;
        g_defence_village = village = find_village(x, y);
        tribe = g_settlements[village].owner - 4;
        if (g_tribes[tribe].level >= LEVEL_ADVANCED) {
            bonus += 2;
            g_combat_flags[1] |= 0x10;
        }
        if (g_settlements[village].flags & SETTLEMENT_CAPITAL) {
            bonus = bonus * 2;
            g_combat_flags[1] |= 0x20;
        }
        g_combat_flags[1] |= 8;
    } else if (is_colony(x, y) >= 0) {
        bonus += 2;
        g_defence_colony = find_colony(x, y);
        set_colony(g_defence_colony);
        bonus += get_colony_defense() * 2;
        g_combat_flags[1] |= 0x40;
    } else {
        terr = terrain_at(x, y);
        if (NATION(g_units[u]) >= 4
            || (an < NATION_EUROPEAN_COUNT && ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 || an >= NATION_EUROPEAN_COUNT || g_nation_text[an].control != CONTROL_HUMAN))) {
            bonus += g_terrain_rows[terr].defence;
            g_combat_flags[1] |= 0x80;
        } else {
            open = 1;
            if (an < NATION_EUROPEAN_COUNT && g_nation_text[an].control == CONTROL_HUMAN) {
                if (is_colony(x, y) >= 0
                    || is_colony(g_units[attacker].x,
                                              g_units[attacker].y) >= 0) {
                    open = 0;
                    bonus += g_terrain_rows[terr].defence;
                    g_combat_flags[1] |= 0x80;
                }
            } else {
            }
            if (open != 0) {
                g_terrain_defence = g_terrain_rows[terr].defence;
                g_combat_flags[0] |= 0x80;
            }
        }
    }
    if (g_units[u].orders == ORDERS_FORTIFIED
        && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
        && bonus < 5) {
        bonus += 2;
        g_combat_flags[1] |= 0x2000;
    }
    str = (bonus + 4) * str >> 2;
    return str;
}
