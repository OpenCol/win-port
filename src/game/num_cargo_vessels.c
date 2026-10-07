/* num_cargo_vessels: candidate for 1048:49a0 (155 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks the unit chain starting at the square DGROUP:0x4cc8 - 20 (both coordinates) and
 * counts the units whose type-info byte (14-byte records at SEG20:0x7acf) is set; those
 * with bit 0x80 of +4 and a type other than 11 are also counted through *damaged -- flags2 bit 0x80 is the damaged bit.
 *
 * THAT WORD IS THE CURRENT NATION INDEX. `set_europe` writes DGROUP:0x4cc8 with
 * the nation and 0x4cc4 with `&g_nations[n]`, so reading it as an x and a y is a shipped
 * bug of the same shape as `job_available`'s, which indexes the NATION records with a
 * COLONY index. The name here follows what the word IS; what this function does with it
 * is the game's own affair.
 *
 * RENAMED 2026-09-26 from chain_count_flagged: it counts the units with holds on the current nation's Europe square -- the ships in port, which is what every reader of its result (g_ships_in_port, DGROUP:0x1fca) takes it for -- and returns through its pointer how many of them are damaged (flags2 bit 0x80, the bit special_kill sets).
 *
 * RENAMED 2026-09-26 from count_ships_in_port: the Mac build's num_cargo_vessels(short*), bound by three independent signals: LAYOUT (on both sides the one function between boycott and nth_europe_vessel), call-sequence alignment (the lone unbound callee in its gap in compute_europe_parameters) and call-graph agreement (3 vs 2). It counts the ships in port and returns the damaged ones through its pointer, which is the short* the Mac signature has; the Mac body is 1.12x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

int num_cargo_vessels(int *damaged)
{
    int u;
    int n = 0;

    *damaged = 0;
    for (u = top_unit_in_square(g_cur_nation - 20, g_cur_nation - 20); u >= 0; u = next_unit(u)) {
        if (g_unit_types[g_units[u].type].holds != 0) {
            if ((g_units[u].flags2 & 0x80) && g_units[u].type != UNIT_ARTILLERY)
                (*damaged)++;
            n++;
        }
    }
    return n;
}
