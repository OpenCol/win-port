/* GameWin_dtor_1038_8f99: candidate for 1038:8f99 (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GameWin::~GameWin()
 *
 * This module's copy of GameWin's destructor (vtable at DGROUP:0x144): the compiler-generated
 * sequence over its two bases, DialogWin (at +0x26, destroyed first) and GRPort.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;
/* The bases' destructors are defined in this file because the shipped calls to them are the
 * compiler's own `push cs / call `, which it only emits for a callee defined in the
 * translation unit; each is the compiler-generated body (see the module's matched copies). */

GameWin::~GameWin()
{
}
