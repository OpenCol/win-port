/* MiniWin_ctor: candidate for 1008:dc4e (93 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MiniWin::MiniWin()
 *
 * The constructor of MiniWin (0x786 bytes, over ColChild, vtable at DGROUP:0x12c): the base
 * and the MenuClass member at +0x77c constructed.
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

MiniWin::MiniWin()
{
}
