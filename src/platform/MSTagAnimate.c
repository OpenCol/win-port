/* MSTagAnimate: candidate for 1070:01c6 (66 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Sets the reserved byte of count palette entries from start, clipped to 256 entries.
 *
 * RENAMED 2026-09-27 from palette_mark_range_reserved: the Mac build's MSTagAnimate, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Sets the flag byte (PC_RESERVED, the animation tag) on count entries from start, clipped at 256; its only caller is CTable_SetAnimate, which is exactly Mac MSTagAnimate's only caller; 3 args, ratio 1.67.
 */

#include "colonization.h"

#include "records.h"

void MSTagAnimate(Palette *pal, int start, int count)
{
    int i;

    if (start + count > 0x100)
        count = 0x100 - start;
    for (i = start; start + count > i; i++)
        pal->e[i].reserved = 1;
}
