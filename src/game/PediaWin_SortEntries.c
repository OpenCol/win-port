/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* PORT: this file's own declarations of VList_Head_1060_68db, VList_Next_1060_68fb removed: the header declares them from the definition (W1 step 3). */
/* PediaWin_SortEntries: candidate for 1060:33b1 (473 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_SortEntries
 * name-source: sibling-build
 *
 * SORTS THE COLONOPEDIA'S LIST BY DISPLAYED NAME, called at the end of `PediaWin_FillStringList`.
 *
 * It takes a `new`-ed array of pointers to the list's items, one per entry, walks the list to
 * fill it, and then selection-sorts IN PLACE: the six-byte triples the items hold are swapped, not
 * the list links, so the list's own order is untouched and only the contents move. The array is
 * deleted on the way out; an allocation failure returns quietly and leaves the list unsorted.
 *
 * The comparison renders both names with `PediaWin_PediaName` into two 80-byte buffers and compares
 * them with `STRICMP`, so the order is the one the player sees and case does not matter. The
 * current best's name is rendered again on EVERY inner iteration rather than once per outer pass, which
 * makes the sort quadratic in name renderings as well as comparisons -- unnoticeable on a list of a
 * few dozen.
 *
 * The three-way swap comes out in two shapes: the two -to-copies go through `F_SCOPY`, while
 * writing the stack temporary back into the record is three plain word moves.
 *
 * RENAMED 2026-09-26 from pedia_sort_list: the Mac build's PediaWin::SortEntries(void): this only, sorting the index list. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
/* PORT: this file's own VListItem, VList removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e) ? 1 : -1];

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" char *PediaWin_PediaName(PediaWin *w, PediaEntry *entry);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void PediaWin_SortEntries(PediaWin *w)
{
    int n;
    PediaEntry **arr;
    PediaEntry *p;
    PediaEntry *best;
    PediaEntry tmp;
    char best_name[0x50];
    char cand_name[0x50];
    register int i;
    register int j;

    arr = new PediaEntry *[w->count];
    if (arr == 0)
        return;
    n = 0;
    for (p = (PediaEntry *)VList_Head_1060_68db((VList *)&w->list); p != 0; p = (PediaEntry *)VList_Next_1060_68fb((VList *)&w->list)) {
        arr[n] = p;
        n++;
    }
    for (i = 0; i < w->count; i++) {
        best = arr[i];
        for (j = i + 1; j < w->count; j++) {
            STRCPY(best_name, PediaWin_PediaName(w, best));
            STRCPY(cand_name, PediaWin_PediaName(w, arr[j]));
            if (STRICMP(cand_name, best_name) < 0)
                best = arr[j];
        }
        if (arr[i] != best) {
            tmp = *arr[i];
            *arr[i] = *best;
            *best = tmp;
        }
    }
    delete arr;
}

