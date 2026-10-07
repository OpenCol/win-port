/* MapWin_ctor: candidate for 1008:dc03 (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin::MapWin()
 *
 * The constructor of MapWin (0x788 bytes, over ColChild, vtable at DGROUP:0x138): nothing
 * beyond the base construction and the vtable pointer.
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

MapWin::MapWin()
{
}
