/* PediaWin_EmptyStringList: candidate for 1060:326b (117 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Empties the list at +0x83e of the object, one item at a time: take the head, run
 * `operator delete` on it, unlink it, take the next head. The count at +0x84e is zeroed once
 * at the end rather than decremented per item.
 *
 * The loop is a `while` with its test at the TOP of the source and the bottom of the code --
 * the shipped jumps straight to the `CMP DWORD [bp-6],0` before entering, which is what a
 * `while` gives where a `do`/`while` would fall into the body first.
 *
 * The head is a 32-bit pointer and is tested as one (`CMP DWORD`), not by its offset half.
 *
 * RENAMED 2026-09-26 from list_delete_items: the Mac build's PediaWin::EmptyStringList(void): this only, deleting every node of the list at +0x83e and zeroing the count. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void VList_Remove(void *list);

/* same object: 4-byte `push cs; call ` */
extern void *VList_Head_1060_68db(void *list);

void PediaWin_EmptyStringList(PediaWin *o)
{
    void *p;

    p = VList_Head_1060_68db(&o->list);
    while (p != 0) {
        __bdele_qnv_rtl(p);
        VList_Remove(&o->list);
        p = VList_Head_1060_68db(&o->list);
    }
    o->count = 0;
}
