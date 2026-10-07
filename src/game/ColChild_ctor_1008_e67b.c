/* ColChild_ctor_1008_e67b: candidate for 1008:e67b (110 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild::ColChild()
 *
 * Module 1008's copy of ColChild's constructor (0x77a bytes, vtable in DGROUP): the GameWin
 * base, the SImageButton member at +0x6ea, and the message flags at +0x75e/+0x75f cleared.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern void DialogWin_RemoveControl(void *list, int id);

/* The bases' and members' constructors are defined in this file because the shipped calls to
 * them are the compiler's own `push cs / call ` (GRPort's and MenuClass's are calls). */

ColChild::ColChild()
{
    active = 0;
    kind = 0;
}
