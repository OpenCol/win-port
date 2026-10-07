/* MiniWin_dtor: candidate for 1008:e3c5 (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MiniWin::~MiniWin()
 *
 * The destructor of MiniWin (over ColChild, vtable at DGROUP:0x12c) with a MenuClass
 * member at +0x77c.
 * ColChild::~ColChild is defined in-file first, for the 4-byte call.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

MiniWin::~MiniWin()
{
}
