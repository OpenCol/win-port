/* DebugWin_dtor: candidate for 1008:d3eb (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DebugWin::~DebugWin()
 *
 * The destructor of DebugWin: nothing beyond the compiler's vtable reset and base
 * destruction (GameWin's module copy, flag 0).
 * GameWin::~GameWin is defined in-file first, for the 4-byte call.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

DebugWin::~DebugWin()
{
}
