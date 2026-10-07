/* random_reseed: candidate for 1040:74eb (29 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `srand(seed & 0x7fff)` on the low half of a 32-bit seed. Both callers build that seed
 * from a position -- the SEG4 one from the current colony's coordinates and the running
 * value at DGROUP:0x4bc2 -- so this is the deterministic per-place seeding that makes a
 * tile's random content the same every time it is looked at. `seed_random_from_clock`
 * (1070:dd1d) is the other one, and it is the only place the clock gets in.
 *
 * The mask is 16-bit (`AND AX,0x7fff` after loading the low word), not `seed & 0x7fffL`,
 * which would be a 32-bit AND.
 *
 * RENAMED 2026-09-26 from seed_random_with: the Mac build's random_reseed(long), bound by LAYOUT. Both builds keep the random module in one order -- Mac randmize, random_reseed(long), random_activate(short), random(short, short); Windows 1040:74cd, 74eb, 7508, 751a -- and the arities line up one for one. random itself stays rand_range: Borland's stdlib.h defines a one-argument random macro.
 */

#include "colonization.h"

void random_reseed(long seed)
{
    srand_rtl((unsigned)seed & 0x7fff);
}
