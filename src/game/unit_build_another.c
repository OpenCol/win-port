/* unit_build_another: candidate for 1028:0a15 (146 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 146, not the census 137. The trailing 9 bytes are the dead second epilogue.
 *
 * Adds three contributions together and clamps the sum at zero. The terrain figure is fetched
 * once, from the square the unit at `u` is standing on, and then handed to two of the three;
 * the third takes only the first argument.
 *
 * THE ADDITIONS ARE AN EXPRESSION, not three statements. Borland has no register to spare
 * across a call, so it pushes each partial sum and pops it back to add the next -- `PUSH AX
 * ... POP DX / ADD DX,AX / PUSH DX` twice over. Written as `r = f(); r += g(); r += h();`
 * the partial sum lands in the frame slot instead and the pushes disappear.
 *
 * The two coordinates are read as unsigned bytes from the unit record and widened, each
 * through its own `IMUL BX,BX,0x1c`.
 *
 * The result is declared BEFORE the terrain figure even though the terrain figure is
 * assigned first: Borland gives the first-declared local the higher frame slot, and the
 * shipped code keeps the sum at `[bp-4]` and the terrain at `[bp-6]`. Declared the other way
 * round the two swap and the function is wrong at one byte in three places.
 *
 * The clamp is `max(0, r)` and not `max(r, 0)`: the shipped pushes the sum FIRST, and 
 * pushes right to left, so the sum is the SECOND argument. Both spellings mean the same
 * thing and only one of them is these bytes.
 *
 * RENAMED 2026-09-26 from route_stop_total: the Mac build's unit_build_another(short, short), bound by call-graph agreement: 2 of its Mac call edges already bound land on this function (caller plan_move; callee land_region_at), the runner-up gets 1; and by call-sequence alignment: in plan_move the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (2), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"

extern int land_region_at(int x, int y);

/* same object: 4-byte `push cs; call ` */
extern int build_another_colony(int a);
extern int region_build_factor(int nation, int region);
extern int unit_build_factor(int a, int u, int t);

int unit_build_another(register int nation, register int u)
{
    int r;
    int region;

    region = land_region_at(g_units[u].x, g_units[u].y);
    r = build_another_colony(nation) + region_build_factor(nation, region) + unit_build_factor(nation, u, region);
    r = max_int_1028_e350(0, r);
    return r;
}
