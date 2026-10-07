/* MSDecodeDeltaFlc: candidate for 1070:86ad (42 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * asm_decode_delta_flc (segment 1080, the WinG layer) with two longs, three words and a byte
 * passed through. `add` is `unsigned char`, as on the Mac: the segment-1080 decoder
 * reads it only as a byte (`ADD AL,[BP+14h]`), and declared `unsigned char` at both ends
 * the pass-through is still one `PUSH [BP+14h]`. Declared `int` here alone it grades
 * DIFFERS -- a `MOV AL / MOV AH,0 / PUSH AX` widening -- so the old `int` was an artefact
 * of the callee's declaration, not the port's width (substitution, 2026-09-28).
 *
 * RENAMED 2026-09-26 from wing_call_0000: Flic_DecodeFrame' handler for FLIC chunk 7, DELTA_FLC ("SS2",
 * word-oriented delta), forwarding to the segment-1080 decoder.
 *
 * RENAMED 2026-09-27 from flic_decode_ss2: Mac MSDecodeDeltaFlc(unsigned char*, unsigned char*, short, short, short, unsigned char), FLIC chunk 7 (SS2). Arity 6 = (body, bits, x, y, stride, add), with the longs as pointers. FLIC DECODER BLOCK: Windows 1070 84c8, 85b2, 86ad, 86d7, 8990, 89ba; Mac CODE 4 MSDecodeColor256, Color64, DeltaFlc, DeltaFli, ByteRun, Literal. The order is EXACT. The single caller of each is Flic_DecodeFrame (1068:1b11), in the seq position of its Mac namesake, 8 of 8 (FLIC chunks 4, 7, 11, 12, 15, 16). NOT PROPOSED BY mac_bind. Sizes differ by implementation: the 42-byte Windows ss2/brun/copy are thunks to assembly, and the Mac DeltaFli is its 40-byte thunk. Arity is the tie-break and agrees on every one.
 *
 * `body` and `bits` are pointers, as its caller passes them (2026-10-02; they were `long`s).
 */

#include "colonization.h"

void MSDecodeDeltaFlc(unsigned char *body, unsigned char *bits, int x, int y, int stride, unsigned char add)
{
    asm_decode_delta_flc((long)body, (long)bits, x, y, stride, add);
}
