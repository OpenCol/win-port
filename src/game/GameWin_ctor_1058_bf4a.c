/* GameWin_ctor_1058_bf4a: candidate for 1058:bf4a (119 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GameWin::GameWin()
 *
 * This module's copy of GameWin's constructor (0xc4 bytes, vtable at DGROUP:0x2674): GRPort
 * and DialogWin constructed, the inherited f8c and its own long at +0xc0 cleared.
 * DialogWin's constructor is defined here for the compiler's `push cs / call ` form.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

GameWin::GameWin()
{
    first_ctl = 0;
    relayout = 0;
}
