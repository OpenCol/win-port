/* CTable_ResetAnimate: candidate for 1068:05cf (55 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_ResetAnimate
 * name-source: sibling-build
 *
 * Fills a range of a logical palette and then installs the same range: first
 * `MSTagNonAnimate(pal, first, count)`, then `CTable_UpdatePal(pal, first, count)`.
 *
 * `first` and `count` are held in SI and DI from entry because each is used twice; under -Od
 * only `register` parameters go there. MSTagNonAnimate is a call into the
 * widget segment; CTable_UpdatePal is the four-byte `push cs; call `, so it is defined
 * here (rule 47).
 *
 * RENAMED 2026-09-27 from palette_fill_and_install: the Mac build's CTable_ResetAnimate, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: MSTagNonAnimate(pal, first, count) then CTable_UpdatePal: the Mac ResetAnimate's MSTagNonAnimate + UpdatePal; its caller CTable_ResetScale has Mac ResetScale's exact shape (HLock, Fill, HUnlock, ResetAnimate, DisposeHandle); this + 2.
 */

#include "colonization.h"

struct logpal;
#include "classes.h"
extern void MSTagNonAnimate(struct logpal *pal, int first, int count);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void CTable_UpdatePal(CTable *pal, int start, int count);

void CTable_ResetAnimate(CTable *pal, register int first, register int count)
{
    MSTagNonAnimate((struct logpal *)pal, first, count);
    CTable_UpdatePal(pal, first, count);
}
