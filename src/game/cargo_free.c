/* cargo_free: candidate for 1018:499d (79 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when the SEG21:0x122 amount for k added to the colony's stock of k
 * passes the SEG21:0x164 limit for k.
 *
 * RENAMED 2026-09-26 from stock_would_overflow: the Mac build's cargo_free(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the fourth of the four functions between net_cargo_change and get_progress, which are already bound, paired in order; the arity agrees and the Mac body is 0.99x this one. Call-sequence alignment in colony_plan had put it here too.
 */

#include "colonization.h"

#include "records.h"

extern int g_consumption[];              /* SEG21:0x164 */

int cargo_free(int goods)
{
    return g_consumption[goods] < g_prod[goods] + g_cur_colony_rec->stock[goods];
}
