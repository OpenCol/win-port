/* neighbor_bits_alt: candidate for 1040:0501 (150 bytes, 1 call site: 1040:05dd).  Verdict: RELOCATABLE.
 *
 * The same routine as reconstructed/neighbor_bits.c (1038:d73b) over a different
 * plane. Diffing the two disassemblies shows the instruction sequences are identical
 * except for three things: the pointer read is at a different pair of DGROUP
 * words, and `g_map_size.w` is reached through an explicit segment load here --
 * `MOV DX,<seg> / MOV ES,DX / IMUL word ptr ES:[..]` -- rather than DS-relative.
 *
 * That last difference is why this file exists separately rather than as a note. The
 * two translation units declare the *same* variable differently: one has it `` and
 * one does not, and under -WS (DS == SS == DGROUP) both reach it correctly. So the
 * six extra bytes are a fact about the header each file included, not about the
 * algorithm -- and changing that one declaration was the only edit needed. Matched
 * first try from the already-verified sibling.
 *
 * Verdict RELOCATABLE.
 * Re-verified 2026-09-10 at the full 150-byte extent, trailing dead epilogue included: RELOCATABLE.
 */

#include "colonization.h"

extern unsigned char *g_draw_feature_loc;   /* DGROUP:0x4c7e */
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

unsigned neighbor_bits_alt(int mask, int unused1)
{
    unsigned result = 0;
    unsigned bit = 1;
    int i;

    for (i = 0; i < 8; i++) {
        if (mask & *(g_draw_feature_loc + (long)(g_dir_dy[i] * g_map_size.w) + g_dir_dx[i]))
            result |= bit;
        bit = bit * 2;
    }
    return result;
}
