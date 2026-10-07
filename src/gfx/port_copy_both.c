/* port_copy_both: candidate for 1068:a9b3 (148 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_copy_both
 * name-source: descriptive
 *
 * Blits one rectangle into BOTH the port at +0xc4 and the object itself: the destination
 * rectangle goes to the port's virtual first slot, the port is selected against the pointer
 * at +2 of the sub-object at +0x26, and then the same four arguments are handed to
 * `GRPort_CopyToPort` twice with different destinations.
 *
 * Two blits from one source is the whole function, and it is why the argument list is
 * repeated instruction for instruction: `-Od` keeps nothing across a call, so all four
 * pointers are pushed again from the frame.
 *
 * The virtual call is why this file is C++ -- a vtable pointer reading a entry,
 * which only `virtual` produces.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MagicWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *get_ctable_1068_d762(void *r);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern "C" void GRPort_ColorSync(GRPort *port, CTable *page);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern "C" void port_copy_both(MagicWin *o, void *src, RECT *sr,
                                             RECT *dr)
{
    o->port.ResizePort(dr);
    GRPort_ColorSync(&o->port, (CTable *)get_ctable_1068_d762((Screen *)&(DialogWin &)*o));
    GRPort_CopyToPort((GRPort *)src, &o->port, sr, dr);
    GRPort_CopyToPort((GRPort *)src, (GRPort *)o, sr, dr);
}
