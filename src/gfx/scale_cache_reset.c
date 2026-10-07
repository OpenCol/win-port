/* scale_cache_reset: candidate for 1068:63c1 (95 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Empties the sixteen-slot scaling-map cache in segment 33 and then asks for the 1x1 map,
 * which re-fills slot 15 (with every stamp 0, SetGlobalSpriteScale's `<=` scan keeps moving `best` to the last slot) and leaves it current. `SetGlobalSpriteScale` -- 95 bytes
 * below, the most-called function of its size in the binary -- is what owns that cache, and
 * this is the only thing that clears it. Its one caller is in segment 1070.
 *
 * Each slot is zeroed in the order `stamp`, `w`, `h`: the 32-bit store at +4 first, then the
 * two words at +0 and +2. That is source order, and `-Od` reorders nothing. Clearing the
 * stamp is what actually empties the cache -- a slot with stamp 0 is always the least
 * recently used -- and clearing `w` and `h` is what stops a 0x0 request matching a stale
 * slot.
 *
 * The index is multiplied by 0x808 three times over, once for each store, because nothing
 * survives in a register between them.
 */

#include "colonization.h"

#include "records.h"
extern ScaleSlot g_scale_cache[];   /* SEG33:0x0000 -- sixteen 0x808-byte slots, the whole 0x8080-byte segment */

/* 5-byte NOP form, so extern (rule 47) */
extern void SetGlobalSpriteScale(int num, int den);

void scale_cache_reset(void)
{
    register int i;

    for (i = 0; i < 16; i++) {
        g_scale_cache[i].stamp = 0;
        g_scale_cache[i].num = 0;
        g_scale_cache[i].den = 0;
    }
    SetGlobalSpriteScale(1, 1);
}
