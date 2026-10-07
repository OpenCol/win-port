/* kill_dest: candidate for 1028:c5f8 (278 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Removes stop `idx` from the current trade route, in two passes.
 *
 * First every unit of the player's own nation that can carry a route and is assigned to
 * THIS route has its stop index pulled back one if it sat at or past the deleted stop,
 * floored at zero through `max_ptr_1028_e5ff` -- which takes two pointers and returns the
 * one holding the larger value, so the floor is a dereference of its result.
 *
 * Then the stops themselves shift down, one struct assignment at a time -- ten bytes each
 * through `F_SCOPY`, from `+0x22 + (i+1)*10` to `+0x22 + i*10` -- and the count byte at
 * +0x21 of the route record drops by one.
 *
 * THE FOUR TESTS ARE NESTED POSITIVE `if`s, not `continue`s. Every one of them is a direct
 * conditional jump to the loop step (rule 12), and a `continue` would give the
 * invert-and-jump-over-a-jump form four times over.
 *
 * THE FLOOR IS TWO COMMA EXPRESSIONS, and this is the only shape that produces the shipped
 * instruction order. `max_ptr_1028_e5ff` takes two `int *` and returns the one holding
 * the larger value, so a floor needs two addressable operands. Written as statements --
 * `lo = s - 1; zero = 0; s = *max_ptr(&zero, &lo);` -- both stores land before either push.
 * The shipped interleaves them:
 *
 *     mov [bp-8],ax / lea ax,[bp-8] / push ss / push ax     <- lo, then its address
 *     mov word [bp-0xa],0 / lea ax,[bp-0xa] / push ss / push ax   <- zero, then its address
 *
 * with the second store BETWEEN the two pushes, which means each store belongs to its own
 * argument's evaluation. `(zero = 0, &zero)` and `(lo = s - 1, &lo)` give exactly that, and
 * they also flip `push ss / lea` into `lea / push ss`, which the statement form does not.
 * A `MAX(a, b)` macro over two scratch ints expands to precisely this; I cannot prove the
 * macro from the bytes, but the comma expressions are what the bytes say.
 *
 * `stops` is at +0x22 and the count byte at +0x21 -- adjacent, with no padding between.
 *
 * RENAMED 2026-09-26 from route_delete_stop: the Mac build's kill_dest(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller delete_colony; callee get_unit_route), the runner-up gets 1, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "units.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int get_unit_route(int u);
extern int get_unit_dest(int u);
extern void set_unit_dest(int u, int v);

/* same object: 5-byte `nop; push cs; call ` -- the linker's conversion */

void kill_dest(int idx)
{
    int r;
    int s;
    int lo;
    int zero;
    register int u;
    register int i;

    for (u = 0; u < g_game.unit_count; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == ((unsigned char)g_game.turn_nation)) {
            if (g_unit_types[g_units[u].type].holds > 0) {
                r = get_unit_route(u);
                if (r == g_cur_route) {
                    s = get_unit_dest(u);
                    if (s >= idx) {
                        s = *max_ptr_1028_e5ff((zero = 0, &zero),
                                               (lo = s - 1, &lo));
                        set_unit_dest(u, s);
                    }
                }
            }
        }
    }
    for (i = idx; g_cur_route_rec->count - 1 > i; i++)
        g_cur_route_rec->stops[i] = g_cur_route_rec->stops[i + 1];
    g_cur_route_rec->count--;
}
