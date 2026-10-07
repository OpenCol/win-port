/* get_unit_loc: candidate for 1048:4df1 (144 bytes, 2 call sites: 1048:3a2f, 1048:9bfc).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * A grid-cell layout calculation: an index is folded into a band number (0, 1 or 2)
 * and a residue, and the residue drives a position `residue * 0x21 + b`. Two of the
 * out-parameters are set to a constant 0x20 and one to a per-band row y (0x138
 * for band 0, 0x16e for band 1, untouched for band 2).
 *
 * Three details the comparator forced, none of them visible in the decompiled C:
 *   - `XOR SI,SI` precedes `MOV CX,[BP+6]`, so the initialisers run in declaration
 *     order and the modified parameter is a *local copy*, not the parameter itself.
 *   - `MOV DI,0x20 / LEA AX,[DI+1] / MOV [BP-4],AX` computes 0x21 from the 0x20
 *     already in a register and parks it in the frame's only local slot. That is a
 *     second named local `cell = w + 1`, not a folded constant -- which is also what
 *     the `SUB SP,0x2` in the prologue is for.
 *   - `MOV AX,DI / MOV ES:[BX],AX / ... / MOV ES:[BX],AX` stores one value twice
 *     through two different pointers, which is a chained assignment. Written as two
 *     separate statements the compiler emits `MOV ES:[BX],DI` twice instead.
 *
 * Verdict EXACT.
 * Re-verified 2026-09-10 at the full 144-byte extent, trailing dead epilogue included: EXACT.
 *
 * RENAMED 2026-09-26 from grid_cell_layout: the Mac build's get_unit_loc(short, short, short*, short*, short*, short*, short*), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between compute_europe_parameters and europe_change_mode, where the Mac gap holds 1 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 0.97x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

void get_unit_loc(int index, int x, int *band, int *pos,
               int *row_y, int *p6, int *p7)
{
    int residue = 0;
    int v = index;
    int w;
    int cell;

    *band = 0;
    if (v < 3)
        residue = v;
    else {
        (*band)++;
        v -= 3;
        if (v < 5)
            residue = v;
        else
            (*band)++;
    }
    w = 0x20;
    cell = w + 1;
    *p6 = *p7 = w;
    *pos = residue * cell + x;
    switch (*band) {
    case 0: *row_y = 0x138; break;
    case 1: *row_y = 0x16e; break;
    }
}
