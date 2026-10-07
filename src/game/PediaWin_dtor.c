/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* PediaWin_dtor: candidate for 1008:df80 (175 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin::~PediaWin()
 *
 * The destructor of PediaWin: the vtable stored (DGROUP:0xf0), `PediaWin_EmptyStringList` on the
 * object, then the four members at +0x83e, +0x818, +0x7f0 and +0x77c destroyed in reverse
 * declaration order, then the ColChild base.
 *
 * PARKED FOR A WHILE ON ONE BYTE, the hidden destructor flag for the member at +0x83e: the
 * shipped pushes **0** where every spelling tried pushed **2**. 2 is what a member gets; 0
 * is what a BASE gets. The resolution is that both are true at once -- the member's own
 * class has a TRIVIAL destructor, so Borland never calls it and instead calls its BASE's
 * destructor directly, which is a base-destructor call and carries the base's flag.
 *
 *     class Base { ... ~Base(); };                // 1068:9d79, the destructor called
 *     class Derived : public Base { };            // no destructor of its own
 *     ...
 *     Derived list;                                    // member at +0x83e
 *
 * The lone `DEC DWORD [SS:0x10]` at +0x33 with no call beside it is the same fact seen from
 * the other side: the unwinder is told about `VList`'s trivial destructor even though there
 * is no code to run. The earlier attempt reproduced that `DEC` with a separate two-byte
 * member and then had no way to get the 0; one member with a trivial destructor over a base
 * with a real one produces both.
 *
 * THAT NARROWS A NAMING QUESTION `LinkedList_PediaEntry_ctor` LEFT OPEN, without closing it.
 * Whatever class that constructor belongs to, this byte says it derives from 1068:9d21's
 * class, adds no data and declares no destructor -- because a member reached through a
 * base-destructor call is a member whose own class has a trivial destructor. The derived
 * class's own name is still unknown; `VListItem_ctor`/`VListItem_dtor` are a different pair
 * at 1068:9c48/9ca0, so the RTTI string counts do not decide it.
 *
 * THE DERIVED CLASS IS `LinkedList<PediaEntry>` (2026-09-28, later): the shipped member
 * descriptor at SEG2:0xfaa8 carries that name, one base, two destructible sub-objects and the
 * destructor 1008:fbd2 (`LinkedList_PediaEntry_dtor`); see LinkedList_PediaEntry_ctor.c.
 *
 * THE CLASSES ARE THE CORPUS'S OWN since 2026-09-28: the base is `VList` (1068:9d21 / 9d79, as
 * VList_ctor.c and VList_dtor.c declare it; it was `ListBase` here) and the derived class the
 * name LinkedList_PediaEntry_ctor.c gives it. The member at +0x818 is a plain `GRPort` -- the
 * destructor called is GRPort::~GRPort (1068:3767) -- where this file had a second class
 * `GRPort2` for it. Neither `ListBase` nor `GRPort2` is a type-name string in the image.
 *
 * TWO OTHER THINGS THIS FILE ESTABLISHED, both still true and both reusable:
 *
 * (Retracted 2026-09-28: this paragraph said `SImageButton` is 0x6e bytes. The shipped type
 * descriptor says 0x74 -- `DlgControl` 0x1a + 8 pad + two 0x26-byte GRPorts + 6 more bytes,
 * as SImageButton_ctor declares it -- and so do the member offsets: +0x77c + 0x74 = +0x7f0.)
 *
 * A lone `DEC DWORD [SS:0x10]` with no destructor call next to it means a member with a
 * trivial destructor, and is worth recognising on sight. Rule 49 is the same counter seen at
 * a `delete`.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;


/* The bases' destructors are defined in this file because the shipped calls to them are the
 * compiler's own `push cs / call `, which it only emits for a callee defined in the
 * translation unit; each is the compiler-generated body (see the module's matched copies). */

#include "records.h"
template <class T> class LinkedList : public VList { public: LinkedList(); };   /* 1008:e84f */
class Trivial { public: char body[2]; Trivial(); ~Trivial() { } };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
extern "C" void PediaWin_EmptyStringList(PediaWin *p);

PediaWin::~PediaWin()
{
    PediaWin_EmptyStringList(this);
}
