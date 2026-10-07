/* PediaWin_ctor: candidate for 1008:dee7 (153 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin::PediaWin()
 *
 * The constructor of PediaWin (0x85a bytes, over ColChild, vtable at DGROUP:0xf0) with an
 * SImageButton at +0x77c, an SScrollBar at +0x7f0, a GRPort at +0x818 and a
 * `LinkedList<PediaEntry>` at +0x83e (1008:e84f is its constructor; the class derives from
 * VList, adds no data, and the shipped member type descriptor names it and its destructor
 * 1008:fbd2): clears the byte at +0x858.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
template <class T> class LinkedList : public VList {
public:
    LinkedList();
};
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

PediaWin::PediaWin()
{
    initialised = 0;
}
