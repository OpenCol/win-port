/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so `list` came 0xE bytes late and the Colonopedia crashed
 * on its first entry (W7). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 25 of the imported file. */
/* PORT: this file's own declarations of VList_Add_1060_693c removed: the header declares it from the definition (W1 step 3). */
/* PediaWin_AddString: candidate for 1060:32e0 (88 bytes, 7 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * name-source: sibling-build
 *
 * Allocates a 6-byte node with `new`, fills its three shorts, and appends it to the list
 * at +0x83e. C++ for the operator new call.
 *
 * RENAMED 2026-09-26 from list_add_triple: the Mac build's PediaWin::AddString(short, short, short): this plus the entry's name word, category and index, appended to the index list at +0x83e; PediaWin_FillStringList calls it for every entry. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e + 58 * (sizeof(void *) - 4)) ? 1 : -1];   /* PORT: +4 a pointer before it on 64-bit (W9) */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void PediaWin_AddString(PediaWin *o, int word, int category, int index)
{
    PediaEntry *n = new PediaEntry;

    n->name = word;
    n->category = category;
    n->index = index;
    VList_Add_1060_693c((VList*)(&o->list), n);
}
