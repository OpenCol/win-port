/* fold_goods_index: candidate for 1040:442f (46 bytes).  Verdict: EXACT.
 *
 * Folds a goods index: 9 and 17 become 8, anything from 8 up loses 15,
 * lower values pass.
 */

#include "colonization.h"

int fold_goods_index(int goods)
{
    if (goods == 0x11 || goods == 9)
        goods = 8;
    else if (goods >= 8)
        goods -= 0xf;
    return goods;
}
