/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 24, 26 of the imported file. */
/* PORT: this file's own declarations of VList_Head_1060_68db, VList_Next_1060_68fb removed: the header declares them from the definition (W1 step 3). */
/* PediaWin_Entry: candidate for 1060:3338 (121 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The item at index idx of the list at +0x83e: NULL when idx is not below the count at
 * +0x84e, else first then idx nexts, stopping early at the end.
 *
 * RENAMED 2026-09-26 from list_item_at: the Mac build's PediaWin::Entry(short): this plus an index, the list entry at that position (NULL past the count). The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
PediaEntry * PediaWin_Entry(PediaWin *o, int idx)
{
    int n = idx;
    PediaEntry *p;

    if (o->count <= n)
        return 0;
    p = (PediaEntry *)VList_Head_1060_68db((VList *)(&o->list));
    while (n > 0 && p != 0) {
        p = (PediaEntry *)VList_Next_1060_68fb((VList *)(&o->list));
        n--;
    }
    return p;
}
