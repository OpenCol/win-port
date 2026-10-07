/* orthogonal_neighbor_bits: candidate for 1038:d68c (175 bytes, 1 call site: 1038:d7c2 -- the same caller as
 * reconstructed/neighbor_bits.c).  Verdict: RELOCATABLE.
 *
 * The four-orthogonal-neighbour counterpart to neighbor_bits.c. Where that one walks
 * eight offsets from a pair of tables, this one reaches the four cells at -stride,
 * +stride, -1 and +1 from a `` cursor and returns a four-bit code, weighting them
 * 8, 4, 2, 1 in that order.
 *
 * `g_tile_map_loc` is a `` pointer held in DGROUP at 0x4c72 (offset) and 0x4c74 (segment),
 * and the subtraction carries into the segment -- `SUB SI,AX / ADC DX,0 /
 * MOV CX,__AHSHIFT / SHL DX,CL / SUB BX,DX` -- so the cursor genuinely crosses
 * segment boundaries and `` cannot express it. `g_map_size.w` is the same DS-relative
 * 0x4c40 word that neighbor_bits.c multiplies by, which is a second function agreeing
 * that 0x4c40 is the row pitch of the same plane.
 *
 * `r += 8` and not `r = 8`: with the accumulator in a register the compound form is a
 * two-byte `ADD DI,0x8`, and although the first one is provably adding to zero, the
 * shipped code still adds. Ghidra prints it as `iVar1 = 8` because it propagated the
 * constant; the bytes did not.
 *
 * Verdict RELOCATABLE. Matched first attempt, from rule 14 alone.
 * Re-verified 2026-09-10 at the full 175-byte extent, trailing dead epilogue included: RELOCATABLE.
 */

#include "colonization.h"

extern unsigned char *g_tile_map_loc;   /* DGROUP:0x4c72 */
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

int orthogonal_neighbor_bits(int mask, int unused1)
{
    int r = 0;

    if (mask & *(g_tile_map_loc - g_map_size.w)) r += 8;
    if (mask & *(g_tile_map_loc + g_map_size.w)) r += 4;
    if (mask & *(g_tile_map_loc - 1))        r += 2;
    if (mask & *(g_tile_map_loc + 1))        r += 1;
    return r;
}
