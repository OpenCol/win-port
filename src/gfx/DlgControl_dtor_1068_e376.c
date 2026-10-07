/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* DlgControl_dtor_1068_e376: candidate for 1068:e376 (115 bytes, 3 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DlgControl::~DlgControl()
 *
 * The destructor of DlgControl as compiled into module 1068: unlink from the parent's
 * control list by id, clear the next link, close the window. A copy of DlgControl_dtor_1050_92a3.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DialogWin, DlgControl removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* DialogWin : EventWin, flattened for C */



/* PORT: this copy of DlgControl::~DlgControl removed: src/game/DlgControl_dtor_1008_ec94.c defines it once (W2). */
