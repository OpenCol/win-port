/* MSAnimatePal: candidate for 1070:027a (55 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * When the palette mode word at DGROUP:0x3380 is 1, animates count entries from start of the
 * palette into the GDI palette handle: AnimatePalette(hpal, start, count, &p->e[start]).
 *
 * RENAMED 2026-09-27 from palette_animate_range: Mac MSAnimatePal(MSCTable*, Palette**, short, short). Caller: CTable::Scale on both (Windows adds palette_animate_own/cross_fade/rotate). Body: AnimatePalette(hpal, start, count, ...). Its signature twin palette_install_range is the SetPaletteEntries one and CTable::UpdatePal's (MSUpdatePalette). Arity 4. PALETTE BLOCK: Windows 1070 palette_get_entry_rgb, palette_set_entry_rgb, palette_set_entry, [create_palette, palette_delete], palette_install_range, copy_logpalette, pack_palette_rgb. The Mac MS palette file has MSGetCTEntry, MSRawSetCTEntry, MSSetCTEntry, MSCopyRawColors, MSTagAnimate, MSTagNonAnimate (both bound), MSInitPalette, MSDestroyPalette, MSAnimatePal, MSUpdatePalette. The two pairs of equal-signature functions separate by caller and by the Win16 call in the body.
 */

#include "colonization.h"

#include "records.h"

void MSAnimatePal(Palette *pal, unsigned hpal, int start, int count)
{
    if (g_palette_mode == 1)
        AnimatePalette(hpal, start, count, &pal->e[start]);
}
