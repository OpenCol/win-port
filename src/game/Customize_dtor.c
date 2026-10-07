/* Customize_dtor: candidate for 1030:ea51 (133 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize::~Customize()
 *
 * The destructor of Customize: nothing beyond the compiler's member and base destruction.
 * SButton::~SButton and GameWin::~GameWin are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

Customize::~Customize()
{
}
