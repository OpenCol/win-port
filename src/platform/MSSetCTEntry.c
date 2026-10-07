/* MSSetCTEntry: candidate for 1070:0342 (256 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Writes (r, g, b) into entry i of a LOGPALETTE with peFlags = 4 (PC_NOCOLLAPSE), then
 * sets peFlags to 1 (PC_RESERVED) when the same colour already exists in
 * the first g_reserved_count entries or in the last g_reserved_count entries -- the
 * system's static colours, which the game does not want to shadow.
 *
 * RENAMED 2026-09-27 from palette_set_entry: Mac MSSetCTEntry(MSCTable*, short, unsigned char x3). Callers: CTable::Fill and CTable::Set on both (Windows adds MSTagNonAnimate). Body: the flagged set. Arity 5. It follows the raw set on both. PALETTE BLOCK: Windows 1070 palette_get_entry_rgb, palette_set_entry_rgb, palette_set_entry, [create_palette, palette_delete], palette_install_range, copy_logpalette, pack_palette_rgb. The Mac MS palette file has MSGetCTEntry, MSRawSetCTEntry, MSSetCTEntry, MSCopyRawColors, MSTagAnimate, MSTagNonAnimate (both bound), MSInitPalette, MSDestroyPalette, MSAnimatePal, MSUpdatePalette. The two pairs of equal-signature functions separate by caller and by the Win16 call in the body.
 */

#include "colonization.h"

#include "w16types.h"
struct logpal { unsigned short version; unsigned short count; PALETTEENTRY entries[1]; };

void MSSetCTEntry(struct logpal *pal, int i, unsigned char r, unsigned char green, unsigned char blue)
{
    int j;

    pal->entries[i].peRed = r;
    pal->entries[i].peGreen = green;
    pal->entries[i].peBlue = blue;
    pal->entries[i].peFlags = 4;
    for (j = 0; j < g_reserved_count; j++)
        if (pal->entries[j].peRed == r && pal->entries[j].peGreen == green && pal->entries[j].peBlue == blue)
            pal->entries[i].peFlags = 1;
    for (j = 256 - g_reserved_count; j < 256; j++)
        if (pal->entries[j].peRed == r && pal->entries[j].peGreen == green && pal->entries[j].peBlue == blue)
            pal->entries[i].peFlags = 1;
}
