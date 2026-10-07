/* menu_popup_parse: candidate for 1070:d2a2 (500 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The other half of the menu grammar, and the half that RECURSES: it fills one popup from the
 * string and hands back where it stopped, so `menu_bar_parse` can carry on with the
 * next popup and a nested `{` here can open a submenu.
 *
 * THE WHOLE GRAMMAR, in one place. A leading `!` makes the item grayed and disabled (flags 3),
 * a `*` checks it (8), then the label runs to the first `}`, `_` or `{`. `_` ends an item and
 * takes the next id from DGROUP:0x39b8; `}` does the same and ends the popup; `{` opens a
 * submenu, which is this function again with a fresh `CreateMenu`, and MF_POPUP is added.
 * Every 0x17th item gets MF_MENUBARBREAK (0x40), which is how a long menu breaks into columns.
 * And a label of exactly `"@SEP"` is thrown away and replaced by MF_SEPARATOR (0x800) with a
 * zero id -- the only string the parser knows by name.
 *
 * `item` at BP-8 is counted and never read. `count` at BP-0x16 is the one that matters.
 *
 * The recursion writes its result back into the PARAMETER `p`, which is why `p` is not a
 * `register`: it has to stay in the frame across the call.
 *
 * Thirteen character tests, every one through `CBW` and `CMP AX,imm` -- thirteen written-out
 * `(int)` casts (rule 51), none of them folded to a byte compare.
 */

#include "colonization.h"

char *menu_popup_parse(unsigned hmenu, char *p)
{
    char done;
    unsigned sub;
    int item;
    char *buf;
    char *start;
    unsigned hmem;
    unsigned id;
    int count;
    register int flags;
    register int n;

    done = 0;
    item = 1;
    count = 0;
    hmem = LocalAlloc(2, STRLEN(p));
    buf = LocalLock(hmem);
    *buf = 0;
    while ((int)*p == ' ')
        p++;
    while (done == 0) {
        if ((int)*p == '!') {
            flags = 3;
            p++;
        } else {
            flags = 0;
        }
        if ((int)*p == '*') {
            flags |= 8;
            p++;
        }
        n = 0;
        start = p;
        while ((int)*p != '}' && (int)*p != '_' && (int)*p != '{') {
            n++;
            p++;
        }
        L1_0c72(buf, start, n);
        if ((int)*p == '_' || (int)*p == '}') {
            if ((int)*p == '}')
                done = 1;
            id = g_next_menu_id;
            g_next_menu_id++;
            p++;
            while ((int)*p == ' ')
                p++;
        } else if ((int)*p == '{') {
            p++;
            sub = CreateMenu();
            p = menu_popup_parse(sub, p);
            flags |= 0x10;
            id = sub;
            while ((int)*p == ' ')
                p++;
            if ((int)*p == '}') {
                done = 1;
                p++;
            }
        }
        count++;
        if (count > 0x16) {
            count = 0;
            flags |= 0x40;
        }
        if (STRCMP(buf, "@SEP") == 0) {
            flags = 0x800;
            *buf = 0;
            id = 0;
        }
        AppendMenu(hmenu, flags, id, buf);
        item++;
        *buf = 0;
    }
    LocalUnlock(hmem);
    LocalFree(hmem);
    return p;
}
