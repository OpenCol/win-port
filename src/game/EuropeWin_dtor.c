/* EuropeWin_dtor: candidate for 1008:e02f (192 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin::~EuropeWin()
 *
 * The destructor of EuropeWin (over ColChild, vtable at DGROUP:0xfc) with an SRegion at
 * +0x77c, three SButtons at +0x7aa/+0x7cc/+0x7ee, an SImageButton at +0x810 and a GRPort at
 * +0x884: nothing beyond the compiler's member and base destruction (module 1008's copies
 * for the controls).
 * SImageButton::~SImageButton, SButton::~SButton, SRegion::~SRegion and ColChild::~ColChild are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

EuropeWin::~EuropeWin()
{
}
