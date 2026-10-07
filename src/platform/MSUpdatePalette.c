/* MSUpdatePalette: candidate for 1070:048e (55 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * When the palette mode word at DGROUP:0x3380 is 1, installs count entries from start of the
 * palette into the GDI palette handle: SetPaletteEntries(hpal, start, count, &p->e[start]).
 *
 * RENAMED 2026-09-27 from palette_install_range: Mac MSUpdatePalette(MSCTable*, Palette**, short, short). Caller: CTable::UpdatePal, 1 of 1 on both. Body: SetPaletteEntries(hpal, start, count, ...), the Win16 counterpart of the Mac update (MacWinActivatePalette). Arity 4. PALETTE BLOCK: Windows 1070 palette_get_entry_rgb, palette_set_entry_rgb, palette_set_entry, [create_palette, palette_delete], palette_install_range, copy_logpalette, pack_palette_rgb. The Mac MS palette file has MSGetCTEntry, MSRawSetCTEntry, MSSetCTEntry, MSCopyRawColors, MSTagAnimate, MSTagNonAnimate (both bound), MSInitPalette, MSDestroyPalette, MSAnimatePal, MSUpdatePalette. The two pairs of equal-signature functions separate by caller and by the Win16 call in the body.
 */

#include "colonization.h"

#include "records.h"

void MSUpdatePalette(Palette *pal, unsigned hpal, int start, int count)
{
    if (g_palette_mode == 1)
        SetPaletteEntries(hpal, start, count, &pal->e[start]);
}
