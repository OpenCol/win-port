/* menu_bar_parse: candidate for 1070:d496 (336 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Builds a whole menu BAR out of one string. The grammar is small: spaces are skipped, a
 * leading `!` means the popup is created disabled, everything up to `{` is the popup's title,
 * and `menu_popup_parse` is handed the rest to fill the popup in and hands back where it
 * stopped. That repeats until the string runs out.
 *
 * `0x13` against `0x10` is the whole of the `!`: MF_POPUP alone, or MF_POPUP with MF_GRAYED and
 * MF_DISABLED.
 *
 * The title is accumulated with `L1_0c72`, the runtime's `strncat` -- it takes both lengths,
 * clamps the count and appends -- into a `LocalAlloc` buffer as long as the whole input, which
 * is cleared again after every popup. `LocalLock` returns a NEAR pointer, so the segment stored
 * beside it is `DS`.
 *
 * `saved = g_next_menu_id` HAPPENS TWICE, once on either side of the first `CreateMenu`, and
 * only the second store can matter. Both are in the shipped code.
 *
 * The id counter is advanced by 0x64 for the whole bar rather than per item, so each menu bar
 * built this way reserves a hundred ids whatever it uses.
 *
 * Every character test goes through `CBW` and `CMP AX,imm`, so each one carries a written-out
 * `(int)` cast (rule 51) -- four of them, none folded.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

unsigned menu_bar_parse(char *s)
{
    unsigned hmem;
    char *p;
    char *buf;
    char *start;
    unsigned saved;
    char enabled;
    unsigned hmenu;
    register int n;
    register unsigned sub;

    p = s;
    saved = g_next_menu_id;
    hmenu = CreateMenu();
    saved = g_next_menu_id;
    hmem = LocalAlloc(2, STRLEN(s));
    buf = LocalLock(hmem);
    *buf = 0;
    while (*p != 0) {
        while ((int)*p == ' ')
            p++;
        if ((int)*p == '!') {
            enabled = 0;
            p++;
        } else {
            enabled = 1;
        }
        n = 0;
        start = p;
        while ((int)*p != '{') {
            n++;
            p++;
        }
        L1_0c72(buf, start, n);
        p++;
        while ((int)*p == ' ')
            p++;
        sub = CreateMenu();
        p = menu_popup_parse(sub, p);
        if (enabled == 0)
            AppendMenu(hmenu, 0x13, sub, buf);
        else
            AppendMenu(hmenu, 0x10, sub, buf);
        *buf = 0;
    }
    g_next_menu_id = saved + 0x64;
    LocalUnlock(hmem);
    LocalFree(hmem);
    return hmenu;
}
