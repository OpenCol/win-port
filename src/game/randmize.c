/* randmize: candidate for 1040:74cd (30 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `srand(MSGetTicks() & 0x7fff)` -- the third and last of the game's seeding routines, and
 * the only one that seeds from elapsed time rather than the wall clock or a position. It
 * sits thirty bytes above `random_reseed` (1040:74eb), which takes its seed as a 32-bit
 * argument, and `seed_random_from_clock` (1070:dd1d) uses `time(NULL)`.
 *
 * The mask is 16-bit on the low word, as in its neighbour.
 *
 * RENAMED 2026-09-26 from seed_random_from_ticks: the Mac build's randmize(void), bound by LAYOUT and the call graph. Both builds keep the random module in one order -- Mac randmize, random_reseed(long), random_activate(short), random(short, short); Windows 1040:74cd, 74eb, 7508, 751a -- and the arities line up one for one. random itself stays rand_range: Borland's stdlib.h defines a one-argument random macro.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

void randmize(void)
{
    srand_rtl((unsigned)MSGetTicks_1040_aaa0() & 0x7fff);
}
