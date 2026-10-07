/* MSDeleteMenuItem: candidate for 1070:d72b (84 bytes).  Verdict: RELOCATABLE.
 *
 * Deletes one item from one submenu, or detaches the whole submenu. `item == -1` means the
 * submenu itself: `RemoveMenu(hmenu, pos - 1, MF_BYPOSITION)`, which unlinks it WITHOUT
 * destroying it, so the caller keeps a usable handle. Any other value deletes that one item
 * from the submenu: `DeleteMenu(sub, item - 1, MF_BYPOSITION)`, which does destroy it.
 * Win16 draws that distinction and this wrapper preserves it; the choice of `RemoveMenu`
 * over `DeleteMenu` in the -1 branch is deliberate, not a synonym.
 *
 * Both indices are one-based and both are converted, as everywhere else in this family.
 *
 * `GetSubMenu` is called BEFORE the branch, so its result is computed and thrown away on
 * the `-1` path. That is what the shipped bytes do -- the call is not inside either arm --
 * and it costs one Win16 call per submenu removal.
 */

#include "colonization.h"

void MSDeleteMenuItem(unsigned hmenu, int pos, int item)
{
    unsigned sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        if (item == -1)
            RemoveMenu(hmenu, pos - 1, 0x400);
        else
            DeleteMenu(sub, item - 1, 0x400);
    }
}
