/* rect12_hit -- shipped at 1030:19b2.
 *
 * Ported by tools/port.py from matched/game/rect12_hit.c.
 *
 * The file's own RECT8, Owner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 rect12_hit(Owner *o, col_i16 x, col_i16 y)
{
    col_i16 found = 0;
    col_i16 i;
    for (i = 0; i < 12; i++) {
        if (((RECT8 *)((unsigned char *)(o) + 0xc4))[i].l <= x && ((RECT8 *)((unsigned char *)(o) + 0xc4))[i].r >= x &&
            ((RECT8 *)((unsigned char *)(o) + 0xc4))[i].t <= y && ((RECT8 *)((unsigned char *)(o) + 0xc4))[i].b >= y) {
            found = i + 1;
            break;
        }
    }
    return found;
}
