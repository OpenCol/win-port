/* neighbor_bits: candidate for 1038:d73b (144 bytes, 1 call site: 1038:d7c2).  Verdict: RELOCATABLE.
 *
 * Eight iterations, each reading a byte from a strided two-dimensional array at
 * `g_tile_feature_loc + row[i] * g_map_size.w + col[i]`, testing it against the caller's mask, and
 * setting bit i of the result. `g_dir_dy` and `g_dir_dx` are eight-entry signed byte tables
 * nine bytes apart in the same segment (SEG27:0x9 and SEG27:0x0).
 *
 * Eight iterations over two small (row, col) offset tables into a strided plane is
 * consistent with a scan of a cell's eight neighbours, and the name says that. Read it
 * as an inference from shape: what the bytes establish is the indexing, the stride and
 * the bit order, not the meaning of the plane.
 *
 * **`g_tile_feature_loc` is a `` pointer, and that is a finding rather than a detail.** The
 * shipped code computes a 32-bit byte offset and then normalises it into `seg:off`
 * with `MOV CX,<reloc> / SHL DX,CL / ADD DX,<segment word>`. The relocated immediate
 * is `ff ff` in the file -- KERNEL's exported `__AHSHIFT` constant, which is in this
 * binary's import table. That is the Windows -array idiom, so `` is the only
 * qualifier that reproduces these bytes; `` computes no carry into the segment.
 * See docs/findings/borland-codegen.md.
 *
 * `bit = bit * 2` is also load-bearing. Borland strength-reduces it to `ADD AX,AX`,
 * while `bit *= 2` emits `MOV AX,2 / IMUL`, `bit += bit` emits an add straight to
 * memory, and `bit = bit + bit` re-reads the operand. Four codegens, one meaning, and
 * only the first, `bit = bit * 2`, matches.
 *
 * Verdict RELOCATABLE.
 * Re-verified 2026-09-10 at the full 144-byte extent, trailing dead epilogue included: RELOCATABLE.
 */

#include "colonization.h"

extern unsigned char *g_tile_feature_loc;   /* DGROUP:0x4c76 */
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

unsigned neighbor_bits(int mask, int unused1)
{
    unsigned result = 0;
    unsigned bit = 1;
    int i;

    for (i = 0; i < 8; i++) {
        if (mask & *(g_tile_feature_loc + (long)(g_dir_dy[i] * g_map_size.w) + g_dir_dx[i]))
            result |= bit;
        bit = bit * 2;
    }
    return result;
}
