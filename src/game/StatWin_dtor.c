/* StatWin_dtor: candidate for 1008:e366 (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: StatWin::~StatWin()
 *
 * The destructor of StatWin (over ColChild, vtable at DGROUP:0x120) with a GRPort member
 * at +0x77a: nothing beyond the compiler's vtable reset, member destruction (flag 2) and
 * base destruction.
 * ColChild::~ColChild is defined in-file first, for the 4-byte call.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

StatWin::~StatWin()
{
}
