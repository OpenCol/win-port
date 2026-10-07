/* in_port: candidate for 1028:b1f8 (169 bytes, 3 call sites).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Tests whether an entry in one array corresponds to an entry in another. Normally it
 * compares the first two bytes of each -- a coordinate pair, on the evidence that they
 * are compared pairwise against a unit's first two bytes. The sentinel index 999 takes
 * a different path: it compares the first byte against a value derived from the fourth
 * (`(flags & 0xf) - 0x14`), which checks the unit against its own nation (x == nation - 0x14)
 * rather than against a colony.
 *
 * WHICH ARRAY IS WHICH, corrected 2026-09-20 by tools/check_global_addresses.py. This
 * file had the two the wrong way round: the 0x1c-stride array was called `g_sites` with
 * no address, and the 0xca-stride one was declared `g_units` at SEG20:0xa208. The
 * shipped code settles it -- `imul bx,bx,0x1c` indexes base -0x5df8, which is 0xa208
 * (g_units), and `imul bx,bx,0xca` indexes base 0x7c28 (g_colonies). So this compares
 * a UNIT against a COLONY, and the old declaration named the colony array after the
 * unit array while pointing it at the unit array's address.
 *
 * Neither the byte match nor the prose above caught it: the offset in a data reference
 * is a masked FIXUP, so `g_units` at the wrong base compiles to the same bytes, and the
 * header had already written 0x7c28 in prose while the declaration said 0xa208.
 *
 * The 0xca-byte-stride array based at 0x1098:0x7c28 is the same one `set_selection`
 * (1018:394a) points its global cursor into, which is a third function agreeing on
 * that array's stride and base.
 *
 * The one divergence: `if (cond) r = 1;` and not `r = (cond);`. The shipped code
 * branches over a `MOV word ptr [BP-4],0x1`, where assigning the comparison's value
 * materialises 0/1 through AX and costs five more bytes. Both express the same thing
 * and only one is what was written -- see rule 17 in docs/findings/borland-codegen.md.
 *
 * The byte arithmetic stays byte-wide because of the (char) cast: `AND DL,0xf` then
 * `ADD DL,0xec`, Borland's own rendering of `- 0x14` on a byte.
 *
 * Verdict RELOCATABLE.
 * Re-verified 2026-09-10 at the full 169-byte extent, trailing dead epilogue included: RELOCATABLE.
 *
 * THE RECORDS ARE THE CORPUS's Unit and Colony; the comparisons are SIGNED. The members are
 * `unsigned char` everywhere else, and this function compares x and y as plain (signed) chars,
 * so each operand carries a `(char)` cast. Comparing the unsigned members directly compiles to
 * different bytes. (Until 2026-10-02 the file kept `struct unit`/`struct colony` char views,
 * which conflicted with the records when segment 6 is merged.)
 *
 * RENAMED 2026-09-26 from site_matches_unit: the Mac build's in_port(short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in perform_orders_route -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

int in_port(int unit_id, int colony_id)
{
    int r = 0;

    if (colony_id == 999) {
        if ((char)g_units[unit_id].x == (char)((g_units[unit_id].flags & 0xf) - 0x14))
            r = 1;
    }
    else if ((char)g_units[unit_id].x == (char)g_colonies[colony_id].x && (char)g_units[unit_id].y == (char)g_colonies[colony_id].y)
        r = 1;
    return r;
}
