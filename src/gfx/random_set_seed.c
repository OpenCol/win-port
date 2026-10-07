/* random_set_seed: candidate for 1068:b6bf (26 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Sets the seed of the game's own generator -- SEG17:0x3f00, a 32-bit LCG stepping
 * `x = x * 0x41c64e6d + 0x3039` at SEG17:0x01ac. `random_below` (1068:b6a2) draws from it
 * through SEG17:0x01dc and `random_get_seed` (1068:b6d9) reads it back, so this address,
 * its two neighbours and the tick-seeder above them are the whole public face of that
 * generator: four forwarders in twenty-six lines of assembly.
 *
 * This generator is NOT the one `random_reseed`, `seed_random_from_clock` and
 * `randmize` reach. Those call `srand_rtl` (1:3d4e), which writes a
 * different seed word at DGROUP:0x47c2 and steps by Borland's own 0x015a4e35. The image
 * carries two independent streams, and this is the one the game itself draws from.
 *
 * The seed is a full 32-bit argument -- `PUSH DWORD [bp+6]`, one instruction under `-3` --
 * where `srand_rtl` takes 16 bits and zeroes the high word.
 */

#include "colonization.h"

void random_set_seed(long seed)
{
    L17_01f0(seed);
}
