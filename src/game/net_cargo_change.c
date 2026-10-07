/* net_cargo_change: candidate for 1018:47dd (140 bytes, 1 call site: 1018:9476).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Reads the same segment-0x10a0 arrays that 1018:4869 writes, and subtracts one of
 * them again through an indirection: a signed `char ` selector at offset 0xa6,
 * used as an index into the array at 0x1b4 when it is not negative. The optional
 * out-parameter receives that same subtracted term.
 *
 * The caller, 1018:9476 (6,636 bytes), is the function carrying the REBELMAJORITY,
 * REBELUNANIMOUS, TORYMINORITY, TORYMAJORITY and SONSUP text identifiers, which
 * places this in the rebel-sentiment reporting. That locates it; it does not name it,
 * so the name here stays structural.
 *
 * Two things the bytes forced:
 *   - `MOV AL,ES:[BX+0xa6] / CBW / OR AX,AX / JL` is the *promoted* comparison. A
 *     plain `if (c >= 0)` on a char compiles to `CMP byte ptr [..],0 / JL` instead;
 *     the explicit (int) cast is what reproduces the sign-extend.
 *   - `CMP dword ptr [BP+8],0` tests the whole pointer, so the parameter is
 *     `int *` and the test is against 0, not against a null.
 *
 * Verdict RELOCATABLE (28 masked bytes, all fixups).
 * Re-verified 2026-09-10 at the full 140-byte extent, trailing dead epilogue included: RELOCATABLE.
 *
 * RENAMED 2026-09-26 from indexed_difference: the Mac build's net_cargo_change(short, short*), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in new_turn_colony -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (8.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

extern int  g_consumption[];   /* SEG21:0x0164 */
extern int  g_shortfall[];   /* SEG21:0x01b4 */

int net_cargo_change(int i, int *out)
{
    int n = g_prod[i] - g_consumption[i];

    if ((int)g_goods_input[i] >= 0) {
        n -= g_shortfall[g_goods_input[i]];
        if (out != 0)
            *out = g_shortfall[g_goods_input[i]];
    }
    return n;
}
