/* SuperWin_dtor: candidate for 1050:6b7d (100 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SuperWin::~SuperWin()
 *
 * The destructor of SuperWin (over GameWin, vtable at DGROUP:0x2536) with an EventWin
 * member at +0xc4: clears the word at DGROUP:0x251e; member and base destruction are the
 * compiler's (both through module 1050's copies).
 * EventWin::~EventWin and GameWin::~GameWin are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

SuperWin::~SuperWin()
{
    g_super_win_open = 0;
}
