/* make_building_table: candidate for 1018:5b07 (489 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * SHUFFLES THE COLONY VIEW'S FIFTEEN BUILDING SITES, and does it from the colony's own seed
 * (`reset_building_sequence`) so the same colony always looks the same. Six passes:
 *
 *  1. SEG21:0x1eb and SEG21:0x1dc are cleared to -1 and the 15 class slots to -1.
 *  2. The five per-kind counters are zeroed.
 *  3. SEG21:0x0 is filled in kind order -- DGROUP:0xbf3 gives each kind's first site and
 *     DGROUP:0xbee how many it has, and the five runs cover the fifteen sites.
 *  4. Each site draws a random FREE entry from its own kind's run, repeating until the
 *     entry is still -1. That is the shuffle, and it only ever moves a site within its kind.
 *  5. Every building class gets a slot, taken from the first building record that names it:
 *     its kind's base plus that kind's running count.
 *  6. Each building the colony HAS -- plus building 0, which is always drawn -- writes its
 *     own index into SEG21:0x1dc at the site its class was shuffled to.
 *
 * `ColonyWin_DrawBuildings` reads the two tables this fills: site i takes its
 * building from SEG21:0x1dc and its kind byte from SEG21:0x0.
 *
 * THE BUILDING RECORD'S FIELD OFFSETS WERE TWO BYTES OUT IN FOUR FILES and the comparator
 * could not see it. `g_buildings[i].family` is one memory reference whose whole displacement --
 * the array's address PLUS the member offset -- is a single fixup, so any member of an extern
 * array compares equal to any other. `setup_building` writes +2..+6 through a
 * `char f0..f6` typedef and pins them; `get_production_cost` pins +7 and +10. Against
 * those, the class byte this function reads is +6 and the kind byte +5, not +8 and +5. A new
 * §119 blind spot, and the four files are corrected beside this one.
 *
 * `(int)g_site_slot[r] >= 0` on the draw: the shipped loads, `CBW`s and tests AX, where the
 * bare `signed char` compare folds into one `CMP BYTE` (rules 51 and 59). The inner count
 * loop is written `g_kind_count[i] > j` and not `j < g_kind_count[i]` -- the shipped loads
 * the count into AX and compares it against SI, which is the left-to-right order.
 *
 * `reset_building_sequence` and `has_building` come through the 4-byte
 * `PUSH CS` form and are stubs here; `rand_range` is a real call (rule 47).
 *
 * `g_site_slot` (SEG21:0x1eb) is the fifteen-entry permutation this function builds: every
 * entry starts -1 and each of the fifteen slots `i` takes a random free position `r` inside
 * its kind's band, `g_site_slot[r] = i`. `building_class_if_own` and `building_class_value`
 * read it back with `g_buildings[b].family` as the index, and it belongs to the same
 * fifteen-element group as `g_site_kind` and `g_site_building`.
 *
 * RENAMED 2026-09-26 from colony_site_layout: the Mac build's make_building_table(void), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in produce_something -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (0).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` */
extern void reset_building_sequence(void);
extern int has_building(int bit);

void make_building_table(void)
{
    int r;
    int i;
    int base;
    int count;
    int cls;
    int kind;
    int used[5];
    int cls_slot[15];
    register int j;
    register int t;

    reset_building_sequence();
    for (i = 0; i < 15; i++) {
        g_site_slot[i] = -1;
        g_site_building[i] = -1;
        cls_slot[i] = -1;
    }
    for (i = 0; i < 5; i++)
        used[i] = 0;
    for (i = 0; i < 5; i++)
        for (j = 0; g_kind_count[i] > j; j++)
            g_site_kind[g_kind_base[i] + j] = i;
    for (i = 0; i < 15; i++) {
        t = g_site_kind[i];
        base = g_kind_base[t];
        count = g_kind_count[t];
        do {
            r = rand_range(0, count - 1) + base;
        } while ((int)g_site_slot[r] >= 0);
        g_site_slot[r] = i;
    }
    for (i = 0; i < 0x2a; i++) {
        cls = g_buildings[i].family;
        if (cls_slot[cls] < 0) {
            kind = g_buildings[i].kind;
            t = used[kind]++;
            t += g_kind_base[kind];
            cls_slot[cls] = t;
        }
    }
    for (i = 0; i < 0x2a; i++) {
        if (has_building(i) != 0 || i == 0) {
            cls = g_buildings[i].family;
            t = cls_slot[cls];
            g_site_building[g_site_slot[t]] = i;
        }
    }
}
