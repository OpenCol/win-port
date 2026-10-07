/* MSCheckMenuItem: candidate for 1070:d6d5 (86 bytes).  Verdict: RELOCATABLE.
 *
 * Ticks or unticks one item of one submenu: `GetSubMenu(hmenu, pos - 1)` then
 * `CheckMenuItem(sub, item - 1, MF_BYPOSITION)` or the same with `MF_CHECKED` (0x408).
 * A null menu handle does nothing.
 *
 * BOTH indices are one-based and both are converted on the way through -- `pos - 1` for
 * the submenu and `item - 1` for the item within it. `MSGetSubMenu` does the same
 * subtraction and its header already records why: "the layer numbers menu positions from
 * 1". This is the second witness for that, and it is the more useful one because it shows
 * the convention is not about submenus specifically -- every menu index this layer takes
 * from its callers is one-based, and every Win16 call it makes is zero-based.
 *
 * The name follows the family: `MS` plus the Win16 call it wraps, as with `MSDestroyMenu`,
 * `MSGetSubMenu`, `MSInitMenu` and `MSTrackPopupMenu`.
 */

#include "colonization.h"

void MSCheckMenuItem(unsigned hmenu, int pos, int item, char checked)
{
    unsigned sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        if (checked == 0)
            CheckMenuItem(sub, item - 1, 0x400);
        else
            CheckMenuItem(sub, item - 1, 0x408);
    }
}
