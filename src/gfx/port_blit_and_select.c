/* port_blit_and_select: candidate for 1068:aa8f (108 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_blit_and_select
 * name-source: descriptive
 *
 * Records the destination rectangle at +0xee, blits into the port at +0xc4, and then selects
 * that port against the bits of the object at +0x26 -- the order matters, because the select
 * is what makes the blit visible and it happens last.
 *
 * `CopyRect` takes the DESTINATION first and is ``, so the object's own rectangle is
 * pushed before the caller's: the caller's rectangle is copied INTO the object, not the other
 * way round.
 *
 * The same destination rectangle then goes to `GRPort_CopyToPort` as its fourth argument while the
 * third is a different one -- source and destination differ here, unlike the three
 * `port_copy_*` helpers above, which pass one rectangle twice.
 *
 * `GRPort_CopyToPort` and `GRPort_ColorSync` are reached by the five-byte NOP form and are
 * `extern`; `get_ctable_1068_d77f` by the four-byte form and is a stub here (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MagicWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *port, CTable *table);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_ctable_1068_d77f(void *p);

void port_blit_and_select(MagicWin *o, void *src, RECT *sr, RECT *dr)
{
    CopyRect(&o->rect, dr);
    GRPort_CopyToPort((GRPort *)src, &o->port, sr, dr);
    GRPort_ColorSync(&o->port, (CTable *)get_ctable_1068_d77f(&(DialogWin &)*o));
}
