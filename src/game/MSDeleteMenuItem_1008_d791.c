/* MSDeleteMenuItem_1008_d791: candidate for 1008:d791 (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSGetTextLength_1010_e2d2`'s twin reaching `MSDeleteMenuItem` instead of `MSGetTextLength`:
 * the word at +0 of the record and two more arguments, passed through.
 *
 * Four bytes shorter than the twin because the `return` is not written out (rule 44). The
 * comparator masks the relocated call word, so the callee's name comes from the
 * disassembly.
 */

#include "colonization.h"

#include "classes.h"

void MSDeleteMenuItem_1008_d791(MenuClass *menu_owner, int pos, int item)
{
    MSDeleteMenuItem(menu_owner->hmenu, pos, item);
}
