/* region_build_factor: candidate for 1028:080e (197 bytes, 4 call sites).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Returns a small bit-coded status for an entry: bits 0-1 carry the three-way sign of
 * a quota (an int table entry divided by 12) against a running total, bit 1 is added
 * when the total is unchanged from its starting byte, and bit 2 when one byte of a
 * 0x10-stride table is zero. The caller reads the result as flags rather than a number.
 *
 * The shipped code compares by subtracting -- `MOV AX,[BP-6] / SUB AX,SI / JLE` rather
 * than `CMP AX,SI` -- twice over, so the source wrote `q - sum > 0` and `q - sum < 0`
 * and not `sum < q` and `q < sum`. Ghidra prints the comparisons; the bytes show the
 * arithmetic. Likewise `CMP byte,1 / JNC` is `< 1` on an unsigned byte and not `== 0`,
 * which would have compared against zero.
 *
 * Rule 12 was very nearly weakened by this function and came out stronger. Its opening
 * `OR DI,DI / JGE +3 / JMP ` jumps 149 bytes, which is outside a short conditional
 * jump's range, so the pair looked as though the compiler might have had no choice.
 * It had one: compiled from a structured `if (p2 >= 0) { ... }` under -3 the same
 * source yields a single `0F 8C` long conditional jump. The pair is a goto at any
 * distance, and the 386 flag is why.
 *
 * Verdict RELOCATABLE.
 * Re-verified 2026-09-10 at the full 197-byte extent, trailing dead epilogue included: RELOCATABLE.
 *
 * RENAMED 2026-09-26 from status_bits: the Mac build's region_build_factor(short, short), bound by call-graph agreement: 3 of its Mac call edges already bound land on this function (caller find_beach; caller plan_move; caller unit_build_another), the runner-up gets 2; and by call-sequence alignment: in plan_move the calls either side align and this is the one unbound callee left in that gap (1.0 votes, next 0.0); the arity agrees (2), and the body does what the Mac name says.
 */

#include "colonization.h"

int region_build_factor(int nation, int region)
{
    int r = 0;
    int quota;
    int sign;
    int sum;
    int i;

    if (region < 0) goto done;
    quota = g_region_size[region] / 12;
    sum = g_region_settlements[region];
    for (i = 0; i < 4; i++)
        sum += g_region_colony_count[i][region];
    sign = (quota - sum > 0) ? 1 : ((quota - sum < 0) ? -1 : 0);
    r += sign;
    if (sum - g_region_settlements[region] == 0)
        r += 2;
    if (g_region_colony_count[nation][region] < 1)
        r += 4;
done:
    return r;
}
