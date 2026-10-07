/* MSAppendMenu: candidate for 1070:db50 (85 bytes).  Verdict: RELOCATABLE.
 *
 * Appends one item to a menu and starts a new COLUMN every 22 items: `GetMenuItemCount`,
 * and when the count is a non-zero multiple of 22 the item gets `MF_MENUBREAK` (0x40).
 * A null menu handle does nothing.
 *
 * 22 is the whole design decision. A Win16 popup menu taller than the screen is unusable,
 * and this layer's menus are built from data (nation lists, unit lists) rather than from a
 * resource, so nothing else bounds their length.
 *
 * REGISTER ALLOCATION (rule 40), and this function is one of the three that established it.
 * The shipped code holds the menu handle in SI and the item count in DI, and the count must
 * be declared FIRST and PLAIN: the first-declared local is auto-enregistered into DI, an
 * explicit `register` promotes a local to SI and displaces the parameter, and a local that
 * is neither first nor `register` goes to the stack. All three spellings were tried before
 * the rule was known, and the one that works is the one nobody tries -- because `register`
 * looks like the way to ask for a register, and asking is what moves it to the wrong one.
 */

#include "colonization.h"

void MSAppendMenu(unsigned hmenu, char *text, unsigned id)
{
    int n;
    unsigned flags;

    if (hmenu != 0) {
        n = GetMenuItemCount(hmenu);
        if (n % 22 == 0 && n != 0)
            flags = 0x40;
        else
            flags = 0;
        AppendMenu(hmenu, flags, id, text);
    }
}
