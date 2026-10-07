/* SScrollBar_ctor: candidate for 1008:e7ec (99 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SScrollBar::SScrollBar()
 * name-source: descriptive
 *
 * The constructor of SScrollBar as compiled into module 1008, 166 bytes above
 * `SScrollBar_dtor` (1008:e892): allocate 0x28 when `this` arrives null, run
 * `DlgControl::DlgControl()`, then set the page size at +0x22 to 1 and clear the change and tracking
 * handlers at +0x1a and +0x1e.
 *
 * The class binding is `PediaWin_dtor`'s, not a guess. `PediaWin_ctor` builds a member at
 * +0x7f0 with this constructor and `PediaWin_dtor` destroys the same +0x7f0 with
 * `SScrollBar_dtor`; the 0x28 it allocates is exactly the span to the next member, the
 * GRPort at +0x818.
 *
 * `sizeof` is `DlgControl`'s 0x1a plus fourteen bytes of its own, and the constructor
 * names ten of them. The store order is the shipped order: the page first, then the two
 * handlers -- source order, since `-Od` reorders nothing.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

SScrollBar::SScrollBar()
{
    page = 1;
    on_change = 0;
    on_track = 0;
}
