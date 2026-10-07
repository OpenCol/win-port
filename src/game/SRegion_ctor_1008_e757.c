/* SRegion_ctor_1008_e757: candidate for 1008:e757 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SRegion::SRegion()
 *
 * Module 1008's copy of the SRegion constructor (0x2e bytes): nothing beyond the DlgControl
 * base, whose constructor is defined here for the `push cs / call ` form.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SRegion::SRegion()
{
}
