/* rec_set_pair_min1: candidate for 1068:7b59 (52 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores two ints at +0x14 and +0x16, each clamped to at least 1. `rec_get_pair`
 * (1068:7b8d, immediately after) reads exactly those two words back, so this is its setter,
 * and it is named to mirror it. On a Sprite the pair sits just past the hotspot at
 * +0x10/+0x12 that `Sprite_GetHotSpot` (immediately before) reads.
 *
 * WHAT THE PAIR IS IS NOT SETTLED: every Sprite declaration calls +0x14 `pad14`. The clamp
 * says only that neither may be zero -- the property of a divisor or a step.
 *
 * `max_int_1068_98e4` is the four-byte `push cs; call ` with no NOP, so it is defined
 * here (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void rec_set_pair_min1(Sprite *r, int a, int b)
{
    r->scale_x = max_int_1068_98e4(a, 1);
    r->scale_y = max_int_1068_98e4(b, 1);
}
