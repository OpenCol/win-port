/* PORT: this file's own declarations of ask_port removed: the header declares it from the definition (W1 step 3). */
/* route_name: candidate for 1028:cb4f (199 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The route editor's "set this stop": whether the cursor is on an existing stop or on the
 * blank line after the last one decides which of two nearly identical bodies runs, and the
 * difference between them is what makes the pair readable.
 *
 * PAST THE END -- the cursor is clamped back to the count, the picker is offered the route's
 * first stop as its current value, and a real answer APPENDS: the count at +0x21 goes up and
 * `new_dest` fills the new entry. Item 0x3e8, the picker's trailing "none"
 * item, is refused here because there is nothing to remove.
 *
 * ON AN EXISTING STOP -- the picker gets that stop as its current value, and the trailing
 * item is offered only when there is more than one stop, because a route with one stop
 * cannot lose it. 0x3e8 then DELETES the stop and anything else overwrites it.
 *
 * `stops > 1` stays a byte compare -- `CMP BYTE [ES:BX+0x21],1` -- because Borland folds the
 * promotion when the constant fits in a byte.
 *
 * Both early exits are rule 12's conditional-over-unconditional pair, so both are `goto`s to
 * the shared tail rather than an `if` wrapping the rest.
 *
 * SIZE: the census 199 is right and the 227-byte gap is not. The 28 bytes between are
 * `choose_cargo`'s exception descriptor, which is data belonging to the NEXT function.
 *
 * RENAMED 2026-09-26 from route_set_stop: the Mac build's route_name(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between dest_get and choose_cargo, which are already bound, paired in order; the arity agrees and the Mac body is 1.18x this one.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
/* same object: 4-byte `push cs; call ` */

extern void new_dest(int colony);
extern void kill_dest(int idx);
extern void route_draw(void);

void route_name(void)
{
    register int colony;

    if (g_cur_route_rec->count <= g_route_stop) {
        g_route_stop = g_cur_route_rec->count;
        set_route(g_route_stop);
        colony = ask_port(g_route_unit, g_cur_route_rec->stops[0].colony, 0, 1);
        if (colony < 0 || colony == 0x3e8)
            goto done;
        g_cur_route_rec->count++;
        new_dest(colony);
    } else {
        set_route(g_route_stop);
        colony = ask_port(g_route_unit, g_cur_route_stop->colony, 0,
                                  g_cur_route_rec->count > 1 ? 1 : 0);
        if (colony < 0)
            goto done;
        if (colony != 0x3e8)
            g_cur_route_stop->colony = colony;
        else
            kill_dest(g_route_stop);
    }
done:
    route_draw();
}
