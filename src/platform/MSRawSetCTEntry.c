/* MSRawSetCTEntry: candidate for 1070:02fe (68 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Stores r, g, b into palette entry i (4-byte entries from +4).
 *
 * RENAMED 2026-09-27 from palette_set_entry_rgb: Mac MSRawSetCTEntry(MSCTable*, short, unsigned char x3). Caller: CTable::Scale on both. Body: stores r, g, b and nothing else. That is the 'raw' set; its twin palette_set_entry adds the reserved-colour peFlags logic. Arity 5. PALETTE BLOCK: Windows 1070 palette_get_entry_rgb, palette_set_entry_rgb, palette_set_entry, [create_palette, palette_delete], palette_install_range, copy_logpalette, pack_palette_rgb. The Mac MS palette file has MSGetCTEntry, MSRawSetCTEntry, MSSetCTEntry, MSCopyRawColors, MSTagAnimate, MSTagNonAnimate (both bound), MSInitPalette, MSDestroyPalette, MSAnimatePal, MSUpdatePalette. The two pairs of equal-signature functions separate by caller and by the Win16 call in the body.
 */

#include "colonization.h"

#include "records.h"

void MSRawSetCTEntry(Palette *pal, int i, char r, char g, char b)
{
    pal->e[i].r = r;
    pal->e[i].g = g;
    pal->e[i].b = b;
}
