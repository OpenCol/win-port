/* ColonyWin_dtor: candidate for 1008:e0ef (217 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin::~ColonyWin()
 *
 * The destructor of ColonyWin (over ColChild, vtable at DGROUP:0x108) with an SRegion, two
 * GRPorts, an array of three SImageButtons, an array of two SButtons and a further
 * SImageButton: nothing beyond the compiler's member and base destruction -- the two arrays
 * go through the runtime's array destructor with the module's destructor copies.
 * SImageButton::~SImageButton, SRegion::~SRegion and ColChild::~ColChild are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

ColonyWin::~ColonyWin()
{
}
