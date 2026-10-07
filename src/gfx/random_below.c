/* random_below: candidate for 1068:b6a2 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 29, not the census 25. The last 4 bytes are an unreachable epilogue;
 * Ghidra sizes by reachable code.
 *
 * Forwards to the helper at SEG17:0x01dc, which is `rand() % n` with a guard that
 * returns without dividing when n is 0. The one caller is `random_in_range`.
 *
 * The generator it draws from is the game's OWN, not the RTL's. SEG17 carries a private
 * 32-bit LCG at 0x3f00 stepping `x * 0x41c64e6d + 0x3039`, and `random_set_seed`,
 * `random_get_seed` and `random_seed_from_ticks` -- the three addresses immediately below
 * this one -- are the rest of its public face. `srand_rtl` (1:3d4e) writes a different seed
 * word at DGROUP:0x47c2 and Borland's `rand` steps it by 0x015a4e35. Two independent
 * streams; `random_reseed` and its two siblings seed the other one.
 */

#include "colonization.h"

int random_below(int n)
{
    return L17_01dc(n);
}
