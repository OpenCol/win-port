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
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void VList_Add_1060_693c(void *list, PediaEntry *item);
extern "C" void PediaWin_AddString(PediaWin *o, int word, int category, int index)
{
    PediaEntry *n = new PediaEntry;

    n->name = word;
    n->category = category;
    n->index = index;
    VList_Add_1060_693c(&o->list, n);
}
