/* music_by_number: candidate for 1040:85ed (139 bytes, 1 call site: 1040:86ac).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A dense 1..0x1a lookup mapping one small identifier to another, compiled to a jump
 * table: `MOV BX,CX / DEC BX / CMP BX,0x19 / JA default / ADD BX,BX / JMP CS:[BX+..]`.
 * The table itself sits past the function's last byte, at CS:0x8678, so it is outside
 * the 135 bytes compared here; its offset in the JMP is one of the two masked fixups.
 *
 * The one divergence worth recording: cases 0x0d-0x12 and 0x13-0x16 compute the same
 * `x + 0x1b`, and the shipped code contains *two identical nine-byte blocks* for them
 * at 1040:8648 and 1040:8651 rather than sharing one. Merging them into a single case
 * list is nine bytes short and moves every jump after it. The original source wrote
 * the two ranges as separate cases with duplicated bodies, and the bytes say so.
 *
 * Verdict RELOCATABLE.
 * Re-verified 2026-09-10 at the full 139-byte extent, trailing dead epilogue included: RELOCATABLE.
 *
 * RENAMED 2026-09-26 from lookup_code: the Mac build's music_by_number(short), bound by call-sequence alignment: in check_music the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

int music_by_number(int x)
{
    int r = 0x25;

    switch (x) {
    case 0x01: r = 0x20; break;
    case 0x02: r = 0x21; break;
    case 0x03: r = 0x22; break;
    case 0x04: r = 0x23; break;
    case 0x05: r = 0x3a; break;
    case 0x06: r = 0x3b; break;
    case 0x07: r = 0x38; break;
    case 0x08: r = 0x24; break;
    case 0x09: r = 0x25; break;
    case 0x0a: r = 0x26; break;
    case 0x0b: r = 0x27; break;
    case 0x0c: r = 0x39; break;
    case 0x0d: case 0x0e: case 0x0f:
    case 0x10: case 0x11: case 0x12:
        r = x + 0x1b; break;
    case 0x13: case 0x14: case 0x15: case 0x16:
        r = x + 0x1b; break;
    case 0x17: r = 0x33; break;
    case 0x18: r = 0x32; break;
    case 0x19: r = 0x35; break;
    case 0x1a: r = 0x36; break;
    }
    return r;
}
