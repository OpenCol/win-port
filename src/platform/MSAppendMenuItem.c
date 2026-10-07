/* MSAppendMenuItem: candidate for 1070:dae6 (106 bytes).  Verdict: RELOCATABLE.
 *
 * `MSAppendMenu` one level down: appends an item to the SUBMENU at `pos`, with the same
 * new-column-every-22-items rule. `GetSubMenu(hmenu, pos - 1)` first -- the position is
 * one-based, as everywhere in this family -- then the count of that submenu decides the
 * `MF_MENUBREAK`, so each submenu breaks on its own length rather than the menu bar's.
 *
 * The result of `AppendMenu` is stored into a `char` local that nothing reads. It is a byte
 * at an odd frame offset, which is how the shipped frame gives it away: `SUB SP,6` for a
 * char at [BP-3] and two words at [BP-6] and [BP-8].
 *
 * REGISTER ALLOCATION (rule 40): the count is declared FIRST and PLAIN, so it takes DI
 * while the menu handle keeps SI. This is the function that produced the rule -- it was the
 * fourth function in a row to diverge on exactly this, and the first where the combination
 * that works was tried.
 */

#include "colonization.h"

void MSAppendMenuItem(unsigned hmenu, int pos, char *text, unsigned id)
{
    int n;
    char r;
    unsigned flags;
    unsigned sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        n = GetMenuItemCount(sub);
        if (n % 22 == 0 && n != 0)
            flags = 0x40;
        else
            flags = 0;
        r = AppendMenu(sub, flags, id, text);
    }
}
