/* MapWin_dtor: candidate for 1008:e424 (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin::~MapWin()
 *
 * The destructor of MapWin (over ColChild, vtable at DGROUP:0x138): nothing beyond the
 * compiler's vtable reset and base destruction.
 * ColChild::~ColChild is defined in-file first, for the 4-byte call.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

MapWin::~MapWin()
{
}
