/* PORT: this file's own declarations of get_unit_region removed: the header declares it from the definition (W1 step 3). */
/* collect_indian_stats: candidate for 1030:2822 (356 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Recounts one tribe from scratch: zero the three per-tribe bytes and the tribe's sixteen
 * region bytes, walk every native settlement and then every unit, and add up what belongs to
 * it. The tribe's own nation number is `tribe + 4` -- the four European powers come first --
 * which is the only place in the function that relationship appears.
 *
 * Settlements give the count at SEG20:0xdc10 and the population total at 0xdc08, and each one
 * also bumps a byte in the SHARED sixteen-entry table at SEG20:0xda60, which is zeroed here
 * too: a per-region settlement count that belongs to no tribe and is overwritten by whichever
 * tribe was tallied last.
 *
 * Units give combat strength, added twice -- once into the tribe's total at SEG20:0xdc00 and
 * once into its own region's byte in the 0x10-wide table at 0xdc18 -- through
 * `plus`, so both saturate rather than wrap. A unit whose tile has no
 * region (-1) is counted in the total but not placed.
 *
 * `g_cur_settlement_rec` is a FAR POINTER HELD IN A FAR VARIABLE: the shipped loads SEG34 into
 * ES before the `LES`, so it is `Settlement * ` and not the `Settlement *` most
 * modules declare. Rule 9.
 *
 * RENAMED 2026-09-26 from tally_natives: the Mac build's collect_indian_stats(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee get_combat; callee land_region_at; callee set_village), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_village(int s);
extern int land_region_at(int x, int y);
extern int get_combat(int u, int attacking);

/* same object: 4-byte `push cs; call ` */
extern void plus(unsigned char *p, int delta);

void collect_indian_stats(register int tribe)
{
    int owner;
    int cell;
    int str;
    register int i;

    owner = tribe + 4;
    g_tribe_strength[tribe] = 0;
    g_tribe_pop[tribe] = 0;
    g_tribe_settlement_count[tribe] = 0;
    for (i = 0; i < 0x10; i++) {
        g_tribe_region[tribe][i] = 0;
        g_region_settlements[i] = 0;
    }
    for (i = 0; g_game.settlement_count > i; i++) {
        set_village(i);
        if (g_cur_settlement_rec->owner == (unsigned char)owner) {
            g_tribe_settlement_count[tribe]++;
            g_tribe_pop[tribe] += g_cur_settlement_rec->size;
            cell = land_region_at(g_cur_settlement_rec->x,
                                             g_cur_settlement_rec->y);
            g_region_settlements[cell]++;
        }
    }
    for (i = 0; g_game.unit_count > i; i++) {
        if (NATION(g_units[i]) == (unsigned char)owner) {
            str = get_combat(i, 1);
            plus(&g_tribe_strength[tribe], str);
            cell = get_unit_region(i);
            if (cell >= 0)
                plus(&g_tribe_region[tribe][cell], str);
        }
    }
}
