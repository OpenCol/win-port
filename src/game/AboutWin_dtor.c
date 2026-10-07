/* AboutWin_dtor: candidate for 1060:7758 (247 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: AboutWin::~AboutWin()
 *
 * The destructor of AboutWin (over GameWin, vtable at DGROUP:0x29e0): an EventWin at +0xc4, a
 * CTable at +0x150, three GRPorts, a Sprite at +0x8bc, an array of 21 Sprites at +0x8dc and a
 * Sprite at +0xb7c, all destroyed by the compiler in reverse order, then the base.
 * EventWin::~EventWin and GameWin::~GameWin are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

AboutWin::~AboutWin()
{
}
