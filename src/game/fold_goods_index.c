/* fold_goods_index -- shipped at 1040:442f.
 *
 * Ported by tools/port.py from matched/game/fold_goods_index.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int fold_goods_index(int v)
{
 if (v == 0x11 || v == 9)
 v = 8;
 else if (v >= 8)
 v -= 0xf;
 return v;
}
