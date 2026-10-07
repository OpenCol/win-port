/* EventWin_dtor_1010_2a99: candidate for 1010:2a99 (87 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventWin::~EventWin()
 *
 * This module's copy of EventWin's destructor: the compiler-generated sequence over its two
 * bases, EventTable (at +0xd, destroyed first) and Screen.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class DlgControl;
/* The bases' destructors are defined in this file because the shipped calls to them are the
 * compiler's own `push cs / call `, which it only emits for a callee defined in the
 * translation unit; each is the compiler-generated body (see the module's matched copies). */

/* PORT: this copy of EventWin::~EventWin removed: src/game/EventWin_dtor_1008_d632.c defines it once (W2). */
