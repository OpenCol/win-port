/* dispose_wing_surface: candidate for 1070:a64b (140 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 140, not the census 135. The trailing 5 bytes are the third epilogue --
 * two `return`s and the end of the function, rule 44's arithmetic exactly.
 *
 * Frees a WinG offscreen surface and the handle it lives in: put the old bitmap back with
 * `SelectObject`, delete the WinG bitmap, delete the memory DC, delete the palette at +4
 * if there is one, then `MSHUnlock` and `MSDisposeHandle` the handle at +0. Returns a 
 * NULL, which is the point of the return value -- both callers store it straight back over
 * the pointer they passed, so the port is left with no surface rather than a dangling one.
 *
 * The record is the `WinGRec` its sibling accessors already describe:
 * `wing_dib_pointer` reads the bitmap at +6, `MSGrafOrient` the state
 * at +0xa. Called only from `GRPort_Init` and `GRPort_dtor`, both on the port's surface
 * pointer at GRPort+0x22.
 *
 * The two GDI groups are guarded separately: +2 is the DC and gates the first three calls,
 * +4 is an independent object and gates its own, so a record with no DC still frees +4.
 *
 * Every field is re-read through its own `LES BX,[bp+6]` -- eight of them -- because at
 * `-Od` nothing survives a `` call.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void *dispose_wing_surface(WinGRec *r)
{
    register unsigned h;

    if (r == 0)
        return 0;
    if (r->hdc != 0) {
        SelectObject(r->hdc, r->oldbm);
        DeleteObject(r->hbitmap);
        DeleteDC(r->hdc);
    }
    if (r->hpal != 0)
        DeleteObject(r->hpal);
    h = r->handle;
    MSHUnlock(h);
    MSDisposeHandle(h);
    return 0;
}
