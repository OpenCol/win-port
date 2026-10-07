/* SetDifficulty_dtor: candidate for 1030:e8b5 (133 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetDifficulty::~SetDifficulty()
 *
 * The destructor of SetDifficulty: nothing beyond the compiler's member and base destruction.
 * SButton::~SButton and GameWin::~GameWin are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

SetDifficulty::~SetDifficulty()
{
}
