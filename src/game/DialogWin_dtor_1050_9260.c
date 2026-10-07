/* DialogWin_dtor_1050_9260: candidate for 1050:9260 (67 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DialogWin::~DialogWin()
 *
 * This module's copy of DialogWin's destructor: nothing beyond the compiler-generated
 * sequence (exception count, base EventWin destroyed, delete when flag bit 0).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class DlgControl;
/* The bases' destructors are defined in this file because the shipped calls to them are the
 * compiler's own `push cs / call `, which it only emits for a callee defined in the
 * translation unit; each is the compiler-generated body (see the module's matched copies). */

DialogWin::~DialogWin()
{
}
