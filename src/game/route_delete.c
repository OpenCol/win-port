/* route_delete: candidate for 1028:d088 (306 bytes).  Verdict: RELOCATABLE.
 *
 * Deletes a trade route. `TRADEDELETE` picks which one and `SUREDELETE` confirms it; a
 * negative pick or any answer but 1 leaves everything alone.
 *
 * Deleting a route renumbers the ones above it, so every unit that could be carrying goods
 * -- the ones whose type has cargo holds, `UnitType+7 > 0`, the wagon trains and ships
 * -- has its route index fixed up. A unit ON the deleted route is taken off it and its
 * orders byte is cleared if it was 2, the "following a route" order. A unit on a HIGHER
 * route has its index decremented to follow the compaction. Then the record array itself is
 * compacted with a struct copy per element and the count comes down by one.
 *
 * Both text identifiers are read out of the image at DGROUP:0x1292 and 0x129e, and
 * analysis/function-anchors.json binds both to this address.
 *
 * Three spellings the bytes fixed.
 *
 * Both early exits are `goto` and not `return` (rule 12): the shipped code has one epilogue
 * and jumps to it.
 *
 * The holds test is `<= 0` and not `== 0`. The shipped inverted branch is `JA` -- an
 * UNSIGNED greater-than -- where `== 0` gives `JNZ`. One byte, and the only thing that
 * distinguishes two spellings that mean the same for an unsigned byte.
 *
 * And the register assignment is the loop counter in DI, the route index in SI, which took
 * declaring `u` FIRST and marking `i` as `register` (rule 40). Worth noting against that
 * rule: with `int r` declared first and `u` marked `register`, `r` did NOT take DI -- it
 * stayed on the stack and DI went unused. So "first-declared gets DI" is not unconditional;
 * the candidate also has to be worth enregistering. The tell is one byte, `XOR DI,DI` at
 * the top of the loop against `XOR SI,SI`.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int route_get(int kind, int route, char *msg);
extern int get_unit_route(int u);
extern void set_unit_route(int u, int r);
extern void set_unit_dest(int u, int f);

extern void set_dest(int n);

void route_delete(void)
{
    int u;
    register int i;
    int r;

    r = route_get(0, 0, "TRADEDELETE");
    if (r < 0)
        goto out;
    set_dest(r);
    set_name_slot_1028_e412(0, (char *)g_cur_route_rec);
    if (show_text_dialog_1028_e48e("SUREDELETE") != 1)
        goto out;
    for (u = 0; u < g_game.unit_count; u++) {
        if (g_unit_types[g_units[u].type].holds <= 0)
            continue;
        i = get_unit_route(u);
        if (i == r) {
            set_unit_route(u, 0);
            set_unit_dest(u, 0);
            if (g_units[u].orders == ORDERS_TRADE_ROUTE)
                g_units[u].orders = ORDERS_NONE;
        } else if (i > r) {
            i--;
            set_unit_route(u, i);
        }
    }
    for (i = r; i < g_game.trade_route_count - 1; i++)
        g_routes[i] = g_routes[i + 1];
    g_game.trade_route_count--;
out:
    ;
}
