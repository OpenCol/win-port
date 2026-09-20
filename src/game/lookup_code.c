/* lookup_code -- shipped at 1040:85ed.
 *
 * Ported by tools/port.py from matched/game/lookup_code.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 lookup_code(col_i16 x)
{
    col_i16 r = 0x25;

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
