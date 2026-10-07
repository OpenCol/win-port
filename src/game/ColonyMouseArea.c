/* ColonyMouseArea: candidate for 1010:4538 (222 bytes, 16 call sites) -- the largest reconstruction so
 * name-source: sibling-build
 * , and EXACT on the first attempt.  Verdict: EXACT.
 *
 * A screen hit test: six rectangles tried in order, each yielding a region code, with
 * the sixth containing a nested rectangle that splits it into codes 7 and 8. Anything
 * outside all of them returns 0x14.
 *
 * Every bound is a strict inequality. `OR DX,DX / JLE` is `x > 0` and not `x >= 1`;
 * `CMP DX,0x190 / JGE` is `x < 0x190` and not `x <= 399`. Ghidra normalises all four
 * forms to the same shape, so the decompiled C cannot tell them apart and the
 * disassembly can: the immediates in the shipped code are the bounds the programmer
 * wrote, off by one from the ones Ghidra prints.
 *
 * Each arm ends `MOV CX,<code> / JMP <epilogue>`, which is an ordinary else-if chain
 * here rather than the goto of rule 12 -- the chain's join *is* the epilogue, so the
 * jump has nowhere else to go, and no conditional-over-unconditional pair appears.
 *
 * The rectangles are left unnamed. Their coordinates place them on a 640x480 screen
 * and their order is the hit-test priority, but which panel each one is belongs to the
 * screen layout and nothing here establishes it.
 *
 * Verdict EXACT.
 * Re-verified 2026-09-10 at the full 222-byte extent, trailing dead epilogue included: EXACT.
 *
 * RENAMED 2026-09-26 from hittest_panel: the Mac build's ColonyMouseArea(short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between colony_map_click and ColonyWin_MiscButtonClick, where the Mac gap holds 1 functions and the Windows one 2, so the pairing inside it is by size and arity (the Mac body is 1.05x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

int ColonyMouseArea(int x, int y)
{
    int r = 0x14;

    if (x > 0 && x < 0x190 && y > 0 && y < 0x109)
        r = 2;
    else if (x > 0 && x < 0x262 && y > 0x186 && y < 0x1b8)
        r = 5;
    else if (x > 0x19e && x < 0x258 && y > 0x110 && y < 0x183)
        r = 4;
    else if (x > 0x1a8 && x < 0x268 && y > 0x24 && y < 0xe4)
        r = 1;
    else if (x > 0 && x < 0xf0 && y > 0x110 && y < 0x183)
        r = 0;
    else if (x > 0xf2 && x < 0x19c && y > 0x110 && y < 0x184) {
        if (x > 0xfa && x < 0x194 && y > 0x161 && y < 0x184)
            r = 7;
        else
            r = 8;
    }
    return r;
}
