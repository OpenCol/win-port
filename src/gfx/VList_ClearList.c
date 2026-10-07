/* VList_ClearList: candidate for 1068:a54d (53 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_ClearList
 * name-source: sibling-build
 *
 * Empties a list: rewind with `VList_Head`, then remove the head over and over while the
 * cursor at +8 is non-null. `VList_dtor` (1068:9d79) is what calls it, which is why a list's
 * destructor is only three instructions long.
 *
 * The cursor is tested as a 32-bit value (`CMP DWORD [es:bx+8],0`): +8 is the pointer
 * VList_Add and VList_Head move (first +0, last +4, cur +8 -- the three longs `VList_ctor`
 * zeroes), not a count.
 *
 * The test is at the BOTTOM with a jump into it, which is a `while` and not a `do`: the list
 * may already be empty when `VList_Head` returns.
 *
 * RENAMED 2026-09-26 from list_clear: the Mac build's VList::ClearList(void): this only, removing the head until the cursor is null; VList_dtor calls it. The width matches.
 */

#include "colonization.h"

#include "classes.h"
/* same object: reached through the 4-byte `PUSH CS / CALL NEAR`, so it is DEFINED here (rule 47) */
extern "C" void VList_Head(VList *l);

/* same object: 4-byte `push cs; call ` */
extern "C" void VList_Remove(VList *l);

void VList_ClearList(VList *l)
{
    VList_Head(l);
    while (l->cur != 0)
        VList_Remove(l);
}
