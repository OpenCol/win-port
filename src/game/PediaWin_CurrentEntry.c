/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 23, 25 of the imported file. */
/* PORT: this file's own declarations of VList_Head_1060_68db, VList_Next_1060_68fb removed: the header declares them from the definition (W1 step 3). */
/* PediaWin_CurrentEntry: candidate for 1060:2af7 (116 bytes, 8 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks the list at +0x83e: the first item, then `count` (+0x84c) steps of next, and
 * returns where it lands; NULL when the count is negative.
 *
 * RENAMED 2026-09-26 from list_item_at_count: the Mac build's PediaWin::CurrentEntry(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between PediaWin_FillStringList and PediaWin_OnOKClick, where the Mac gap holds 3 functions and the Windows one 4, so the pairing inside it is by size and arity (the Mac body is 0.84x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
PediaEntry * PediaWin_CurrentEntry(PediaWin *o)
{
    PediaEntry *r = 0;
    int i;

    if (o->sel >= 0) {
        r = (PediaEntry *)VList_Head_1060_68db((VList *)(&o->list));
        for (i = 0; o->sel > i; i++)
            r = (PediaEntry *)VList_Next_1060_68fb((VList *)(&o->list));
    }
    return r;
}
