/* build_item_kind -- shipped at 1018:82be.
 *
 * Ported by tools/port.py from matched/game/build_item_kind.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 build_item_kind(col_i16 item, col_i16 *idx)
{
    col_i16 kind = 0;
    col_i16 i;

    if (item < 0) {
        kind = 0;
        i = 0;
    } else if (item < 0x2a) {
        kind = 1;
        i = item;
    } else {
        i = item - 0x2a;
        if (i < 7) {
            kind = 2;
            i += 11;
        }
    }
    if (idx != 0)
        *idx = i;
    return kind;
}
