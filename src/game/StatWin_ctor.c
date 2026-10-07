/* StatWin_ctor: candidate for 1008:dcab (93 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: StatWin::StatWin()
 *
 * The constructor of StatWin (0x7a0 bytes, over ColChild, vtable at DGROUP:0x120): the base
 * and the GRPort member at +0x77a constructed.
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

StatWin::StatWin()
{
}
