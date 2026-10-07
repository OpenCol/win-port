/* LinkedList_PediaEntry_ctor: candidate for 1008:e84f (67 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: LinkedList<PediaEntry>::LinkedList<PediaEntry>()
 * name-source: sibling-build
 *
 * Allocates a 12-byte object when `this` arrives null, runs the constructor at 1068:9d21
 * on it, counts it and hands it back -- a constructor whose whole body is its base class.
 *
 * `PediaWin` is the only user: `PediaWin_ctor` builds a member at +0x83e with this and
 * `PediaWin_dtor` destroys the same +0x83e with the destructor at 1068:9d79. Those two
 * addresses are the corpus's `VList_ctor` and `VList_dtor`, and the twelve bytes match the
 * three longs `VList_ctor` zeroes.
 *
 * WHAT ITS CLASS IS REMAINS OPEN, and `PediaWin_dtor_1008_df80` narrowed it without
 * closing it. That destructor reaches the member at +0x83e through 1068:9d79 -- the
 * destructor belonging to the constructor this one calls -- and passes the hidden flag
 * **0**, which is the flag a BASE gets and not the 2 a member gets. A member destroyed
 * through a base-destructor call is a member whose OWN class has a trivial destructor over a
 * base that has a real one. So whatever class this constructor belongs to, it derives from
 * 1068:9d21's class, adds no data and declares no destructor.
 *
 * That is as as the bytes go. An earlier version of this header argued from the RTTI
 * type-name strings -- `VList` appearing once in segment 1008 and `VListItem` twice in 1068
 * -- that the pair at 1068:9d21/9d79 must be `VListItem`'s and this must be `VList`'s. The
 * two occurrences in 1068 are already accounted for: `VListItem_ctor` and `VListItem_dtor`
 * are a DIFFERENT pair, at 1068:9c48 and 1068:9ca0, and the knowledge base's validator is
 * what caught the collision. The argument was wrong because it counted string references
 * without checking which functions made them.
 *
 * So the name here follows the base class it runs, which is the only relationship the bytes
 * establish; the derived class's own name is not known.
 *
 * WHAT THE REST OF THE CORPUS SAYS, which this header did not take into account. The base
 * pair it runs, 1068:9d21 and 1068:9d79, are themselves reconstructed: `matched/gfx/VList_ctor.c`
 * and `matched/gfx/VList_dtor.c`. Those two declare that class `VList`, with the same three
 * longs this file calls `VListItem`, and four more files -- VList_insert, VList_Add,
 * VList_Remove, PediaWin_ctor -- declare the same 12 bytes as `VList` with the three
 * longs spelled as the `first`, `last` and `cur` pointers of a list head.
 * (2026-10-02: every file now declares them as those pointers, `VListItem *`.)
 *
 * A function's own reconstruction outranks any other file's declaration of it, so the class at
 * 1068:9d21 is `VList`. The names in THIS file are therefore shifted by one: its `VListItem` is
 * the corpus's `VList`, and the derived class at 1008:e84f -- which adds no data and declares no
 * destructor -- is the one with no name. The file's own name and its `symbol:` line inherit that
 * shift and claim more than is known.
 *
 * 2026-09-28: the shift is now taken out of the CODE. tools/check_data_refs.py resolved the
 * base-constructor call's relocation: the shipped call at 1008:e87b names 1068:9d21
 * (`VList::VList()`), where `VListItem::VListItem()` -- the symbol this file used to spell for
 * the base -- is at 1068:9c48. So the base class below is `VList`, and the derived class, whose
 * real name is not known (the Mac build has no class deriving from VList; PediaWin's member at
 * +0x83e is the only instance), is spelled `VListDerived_1008_e84f` after this address so that
 * its constructor symbol collides with nothing in the corpus. The file name and the record name
 * are unchanged; the `symbol:` line follows the class spelled here.
 *
 * 2026-09-28, later: THE DERIVED CLASS IS `LinkedList<PediaEntry>`. tools/check_data_refs.py
 * follows PediaWin's type descriptor to its member at +0x83e: the shipped member descriptor at
 * SEG2:0xfaa8 is 0x0c bytes, has one base and two destructible sub-objects, names the
 * destructor 1008:fbd2 (`LinkedList_PediaEntry_dtor`) and carries the string
 * `LinkedList<PediaEntry>`. So this constructor is that template instance's; it adds no data
 * and declares no destructor, which is why PediaWin's destructor tears the member down through
 * `VList::~VList` with flag 0, and why 1008:fbd2 has the compiler's implicit-destructor shape.
 * PediaWin_ctor and PediaWin_dtor declare the member that way (`template <class T> class
 * LinkedList : public VList { LinkedList(); }`), and so does the code below.
 *
 * 2026-09-28: THE REAL SPELLING. omf.resolve now reads Borland's template mangling, so the
 * `symbol:` line names the instance and finds the PUBDEF `@%LinkedList$t10PediaEntry%@$bctr$qv`.
 * It is spelt as TDUMP prints it, `LinkedList<PediaEntry>::LinkedList<PediaEntry>()`, so that
 * `batch_match --validate-omf` (which looks the symbol up in TDUMP's listing by exact text)
 * agrees too; `LinkedList<PediaEntry>::LinkedList()` and Borland's own
 * `%LinkedList$t10PediaEntry%::LinkedList()` resolve to the same PUBDEF. The raw name's 36
 * characters never matter: TDUMP prints the demangled name whole, the graders read the PUBDEF
 * from the object with omf.py, and check_reconstructions rule 2 only limits a symbol with
 * neither `::` nor an argument list. The placeholder class
 * `VListDerived_1008_e84f` is gone, and check_data_refs now resolves PediaWin_ctor's call
 * to this constructor (1008:df61 -> 1008:e84f) through this PUBDEF.
 *
 * RENAMED 2026-09-27 from VList_ctor_1008_e84f: the class is LinkedList<PediaEntry> (it derives from VList and adds no data): PediaWin's member type descriptor names it and its destructor LinkedList_PediaEntry_dtor (1008:fbd2), and this is its constructor, spelled LinkedList<PediaEntry>::LinkedList<PediaEntry>(). The name follows the destructor's.
 */

#include "colonization.h"

#include "classes.h"
#include "records.h"
template <class T> class LinkedList : public VList {
public:
    LinkedList();
};

LinkedList<PediaEntry>::LinkedList()
{
}
