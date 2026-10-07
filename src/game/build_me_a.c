/* build_me_a: candidate for 1018:0438 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Clears bit 7 of the current colony's byte at +0x1c and returns its argument
 * plus 0x1f as a char -- the return value is whatever AL held, which the
 * source computed on the way out.
 *
 * RENAMED 2026-09-26 from clear_colony_flag_1c: the Mac build's build_me_a(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between needs_a_building and colony_plan, which are already bound, paired in order; the arity agrees and the Mac body is 1.16x this one.
 */

#include "colonization.h"

#include "records.h"

char build_me_a(int n)
{
    /* declared HERE, at block scope: this function reaches them with a segment
       fixup while the rest of segment 4 reads the same globals through DS, and one
       C++ module can hold both views only this way */

    g_cur_colony_rec->status &= 0x7f;
    return n + 0x1f;
}
