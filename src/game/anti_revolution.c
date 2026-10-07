/* anti_revolution: candidate for 1038:4f6d (58 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Gives the four European nations their default map colours: 10, 11, 12 and 13.
 *
 * The four bytes at DGROUP:0x9a..0x9d are `g_nation_colour[0..3]`, which ten files index
 * by nation -- `colour = g_nation_colour[nation]` in `PieceDraw_noport`, and
 * `tnation < 4 ? g_nation_colour[tnation] : g_tribe_colour[tnation - 4]` in the reports,
 * which is also where the bound of four comes from. This file used to declare the other
 * three as separate `g_order_9*` bytes, and nothing else ever named them.
 *
 * RENAMED 2026-09-26 from reset_nation_colours: the Mac build's anti_revolution(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between purge_square and get_strength, which are already bound, paired in order; the arity agrees and the Mac body is 1.07x this one.
 */

#include "colonization.h"

void anti_revolution(void)
{
    g_nation_colour[0] = 10;
    g_nation_colour[1] = 11;
    g_nation_colour[2] = 12;
    g_nation_colour[3] = 13;
}
