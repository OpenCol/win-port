/* MSReleaseFont: candidate for 1078:134c (79 bytes).  Verdict: RELOCATABLE.
 *
 * Locks the font handle, deletes the GDI font when the record says it owns one (+2 == 1),
 * unlocks and disposes the handle.
 */

#include "colonization.h"

#include "records.h"

void MSReleaseFont(unsigned hfont)
{
    FontRec *font = (FontRec *)MSHLock(hfont);

    if (font->owned == 1)
        DeleteObject(font->hfont);
    MSHUnlock(hfont);
    MSDisposeHandle(hfont);
}
