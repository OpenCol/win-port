/* palette_animate_own: candidate for 1068:0606 (42 bytes).  Verdict: RELOCATABLE.
 *
 * Animates count entries of a palette from start, into the palette's own GDI handle at +0x404:
 * the three-argument form of MSAnimatePal, which takes the handle separately.
 */

#include "colonization.h"

#include "records.h"
extern void MSAnimatePal(Palette *pal, unsigned hpal, int start, int count);

void palette_animate_own(Palette *pal, int start, int count)
{
    MSAnimatePal(pal, pal->hpal, start, count);
}
