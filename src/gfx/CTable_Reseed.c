/* CTable_Reseed: candidate for 1068:0563 (53 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_Reseed
 * name-source: sibling-build
 *
 * Fills the word at +0x406 with a non-zero 15-bit random value.
 *
 * RENAMED 2026-09-26 from seed_nonzero_random: the Mac build's CTable::Reseed(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between CTable_UpdatePal and CTable_SetAnimate, where the Mac gap holds 4 functions and the Windows one 8, so the pairing inside it is by size and arity (the Mac body is 1.17x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void CTable_Reseed(CTable *o)
{
    o->seed = 0;
    while (o->seed == 0)
        o->seed = rand_rtl() & 0x7fff;
}
