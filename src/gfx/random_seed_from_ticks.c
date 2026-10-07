/* random_seed_from_ticks: candidate for 1068:b6f0 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Seeds the game's own generator from the millisecond tick count:
 * `random_set_seed(MSGetTicks())`, written straight through to the SEG17 setter rather
 * than through `random_set_seed` next door. All 32 bits of the tick count go in -- the
 * `PUSH DX / PUSH AX` after the call is the whole `long`, not a truncation.
 *
 * The RTL stream has its own tick seeder, `randmize` (1040:74cd), and that
 * one masks to `& 0x7fff`. Two generators, two tick seeders, one per stream; see
 * `random_set_seed` for why they are separate.
 *
 * `MSGetTicks_1068_d953` is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is
 * in this translation unit and appears here as a stub (rule 47); the SEG17 setter is a 
 * call and is `extern`.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

void random_seed_from_ticks(void)
{
    L17_01f0(MSGetTicks_1068_d953());
}
