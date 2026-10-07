/* PORT: this file's own declarations of CTable_ResetAnimate removed: the header declares it from the definition (W1 step 3). */
/* CTable_ResetScale: candidate for 1068:09c3 (152 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_ResetScale
 * name-source: sibling-build
 *
 * Installs a colour table's packed RGB into the palette and then throws the RGB away: lock
 * the handle at +0x41c, `CTable_Fill` over the entries the table names at +0x40a and
 * +0x40c, unlock, hand the same range to `CTable_ResetAnimate`, and free the handle -- writing
 * `MSDisposeHandle`'s zero back over it, so the table describes itself correctly afterwards.
 *
 * `CTable_load_rgb` is what fills that handle and `CTable_dtor` is what frees it when the
 * table dies; this is the path where the RGB is consumed and released while the table lives
 * on.
 *
 * The unlock's return value is stored into the same local the lock's went to and never read
 * again -- a dead store the source spells `p = MSHUnlock(h)` and Borland keeps because
 * `-Od` removes nothing.
 *
 * `CTable_Fill` and `CTable_ResetAnimate` are reached by the four-byte `PUSH CS / CALL NEAR`
 * form and are stubs here (rule 47); the three MS helpers are calls and are `extern`.
 *
 * RENAMED 2026-09-27 from ctable_install_and_free: the Mac build's CTable::ResetScale(void): it reinstalls the table's saved colours (lock, Fill, unlock), resets the animation range and disposes the saved handle -- the Mac call sequence.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* PORT: an opaque `typedef struct { char pad[1]; } logpal;` removed: the header declares
   struct logpal, which in C++ names the type already (W1). */

/* same object: 4-byte `push cs; call ` */
extern "C" void CTable_Fill(CTable *pal, int start, int count, void *rgb);

void CTable_ResetScale(CTable *pal)
{
    void *p;

    p = MSHLock(pal->hsrc);
    CTable_Fill(pal, pal->first, pal->count, (unsigned char *)p);
    p = MSHUnlock(pal->hsrc);
    CTable_ResetAnimate(pal, pal->first, pal->count);
    pal->hsrc = MSDisposeHandle(pal->hsrc);
}
