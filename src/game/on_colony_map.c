/* on_colony_map: candidate for 1038:b52a (157 bytes, 1 call site: 1018:604c).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * A range test with four modes. Both offsets are folded to their magnitudes and then
 * one to four increasingly generous predicates are OR-ed together, with `mode`
 * selecting where to stop: mode 1 is the four-neighbour test (|dx|+|dy| <= 1), mode 2
 * adds the 3x3 box, mode 3 adds |dx|+|dy| <= 2, and anything else also accepts
 * |dx| < 2 OR |dy| < 2.
 *
 * Two things the bytes forced, both of which change what the source is understood to
 * say rather than merely how it was written:
 *
 *   - `NOT AX / INC AX`, not `NEG AX`. The source negates with `~x + 1`; writing `-x`
 *     compiles to a two-byte `NEG` and diverges at +0x1b. Ghidra printed `~param_1 + 1`
 *     literally and it was right to.
 *   - `CMP SI,1 / JNZ +2 / JMP <epilogue>` is a **goto**. An `if (mode == 1) return r;`
 *     inlines the epilogue instead, and a nested `if (mode != 1) { ... }` emits a plain
 *     `JZ end` -- two bytes where the shipped code has four. Only `goto done;` with a
 *     single labelled `return` at the end reproduces the conditional-over-unconditional
 *     pair, three times over.
 *
 * The ternaries are ternaries and not if-statements: both arms load the value into AX
 * and the store `MOV DX,AX` happens once after the join, so the true arm contains a
 * redundant `MOV AX,DX` that an if-statement would not produce.
 *
 * Verdict EXACT.
 * Re-verified 2026-09-10 at the full 157-byte extent, trailing dead epilogue included: EXACT.
 *
 * RENAMED 2026-09-26 from in_range_mode: the Mac build's on_colony_map(short, short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between on_map and on_screen, where the Mac gap holds 1 functions and the Windows one 2, so the pairing inside it is by size and arity (the Mac body is 1.11x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

int on_colony_map(int dx, int dy, int mode)
{
    int r;

    dx = dx > 0 ? dx : ~dx + 1;
    dy = dy > 0 ? dy : ~dy + 1;
    r = (dx + dy <= 1);
    if (mode == 1) goto done;
    r |= (dx < 2 && dy < 2);
    if (mode == 2) goto done;
    r |= (dx + dy <= 2);
    if (mode == 3) goto done;
    r |= (dx < 2 || dy < 2);
done:
    return r;
}
