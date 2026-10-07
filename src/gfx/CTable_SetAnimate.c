/* PORT: this file's own declarations of MSTagAnimate removed: the header declares it from the definition (W1 step 3). */
/* CTable_SetAnimate: candidate for 1068:0598 (55 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_SetAnimate
 * name-source: sibling-build
 *
 * Marks count entries from start reserved, then installs them.
 *
 * RENAMED 2026-09-26 from palette_mark_and_install: the Mac build's CTable::SetAnimate(short, short): this plus a range, marked reserved (what AnimatePalette requires) and installed. It sits in the CTable module (1068:0000..0d94) and the width matches.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void CTable_UpdatePal(CTable *p, int start, int count);
void CTable_SetAnimate(CTable *p, int start, int count)
{
    MSTagAnimate((Palette *)p, start, count);
    CTable_UpdatePal(p, start, count);
}
