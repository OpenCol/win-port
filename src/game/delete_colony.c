/* delete_colony: candidate for 1018:5e8b (449 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * REMOVES COLONY c AND RENUMBERS EVERYTHING THAT POINTED PAST IT. The owner's colony count goes
 * down, plane-1 bit 1 is cleared on its square, the 0xca-byte records above it slide down one
 * -- a plain struct assignment, which Borland does with `F_SCOPY` and a `CX` of 0xca -- and the
 * colony count follows.
 *
 * Then every trade route's stops are walked BACKWARDS: a stop at the deleted colony is removed,
 * one above it is decremented, and 999 (no colony) is left alone. Backwards because
 * `kill_dest` shifts the stops after it. Finally every unit of a European nation whose
 * home colony byte (+6) is the deleted one gets 0xff, and one above it is decremented.
 *
 * THE PARAMETER IS REUSED AS THE UNIT LOOP COUNTER, which is why the colony index is copied into
 * a local first: the shipped keeps `c` in SI throughout and counts units down in the same
 * register. For that, `j` must be a PLAIN local the allocator promotes and not a `register` one
 * -- a `register` local outranks a `register` parameter and would take SI from it (rule 53).
 *
 * The home-colony comparisons are `(char)cc`: the shipped compares AL against the low byte of
 * the copy, signed (`JNG`), where an int comparison would widen the field first.
 *
 * RENAMED 2026-09-26 from colony_delete: the Mac build's delete_colony(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller Show; caller combat_fight; caller new_turn_colony; callee set_route), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "terrain.h"
#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void feature_set(int x, int y, int mask, int on);
extern void set_dest(int r);

extern void kill_dest(int slot);
extern void DrawMap(int flag);

void delete_colony(register int c)
{
    int i;
    int cc;
    int nation;
    int j;

    nation = g_colonies[c].nation;
    g_nation_colony_count[nation]--;
    feature_set(g_colonies[c].x, g_colonies[c].y, MAP1_SETTLEMENT, 0);
    for (i = c; g_game.colony_count - 1 > i; i++)
        g_colonies[i] = g_colonies[i + 1];
    g_game.colony_count--;
    for (i = 0; g_game.trade_route_count > i; i++) {
        set_dest(i);
        for (j = g_cur_route_rec->count - 1; j >= 0; j--) {
            set_route(j);
            if (g_cur_route_stop->colony != 999) {
                if (g_cur_route_stop->colony == c)
                    kill_dest(j);
                else if (g_cur_route_stop->colony > c)
                    g_cur_route_stop->colony--;
            }
        }
    }
    cc = c;
    for (c = g_game.unit_count - 1; c >= 0; c--) {
        if ((unsigned char)(g_units[c].flags & UNIT_NATION_MASK) < 4) {
            if (g_units[c].home == (signed char)cc)
                g_units[c].home = 0xff;
            else if (g_units[c].home > (signed char)cc)
                g_units[c].home--;
        }
    }
    DrawMap(1);
}
