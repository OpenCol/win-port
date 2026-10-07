/* MagicWin_dtor: candidate for 1068:a70b (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin::~MagicWin()
 *
 * The destructor of MagicWin: nothing beyond the compiler's member (GRPort) and base
 * destruction.
 * GameWin::~GameWin is defined in-file first, for the 4-byte call.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

MagicWin::~MagicWin()
{
}
