/* MSCopyRawColors: candidate for 1070:04ea (169 bytes, 6 call sites).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Packs a Windows `LOGPALETTE`'s entries into consecutive three-byte RGB triples in a
 * `` destination buffer, starting at entry `first` and copying `count` of them.
 * `peFlags` is dropped, which is what makes the output three bytes per entry rather
 * than four.
 *
 * The `LOGPALETTE` is not a guess dressed up as one. The shipped code reads bytes at
 * `pal + i*4 + 4`, `+5` and `+6`, and reaches the base with `LES SI,[BP+6]` -- the
 * whole pointer at once -- rather than adding the pointer's offset into the index
 * register. That is the shape of `p->array[i].field` on a struct pointer, and a
 * four-byte entry array beginning at offset 4 with three consecutive bytes read from
 * it is exactly `LOGPALETTE` { WORD palVersion; WORD palNumEntries; PALETTEENTRY
 * entries[]; } with `PALETTEENTRY` { peRed; peGreen; peBlue; peFlags; }. The field
 * order the bytes require is red, green, blue -- which is `PALETTEENTRY`'s order and
 * the reverse of `RGBQUAD`'s, so the two are distinguishable here and this is the
 * former.
 *
 * This corroborates `tools/palette_fit.py`, which scored all 75 shipped palettes
 * against the sprites, found none that fits, and concluded that the palette is built
 * at run time. A routine whose input is a live `LOGPALETTE` rather than a shipped
 * resource is what that conclusion predicts.
 *
 * Four attempts went into the register allocation before the cause turned out not to
 * be register allocation at all; see rule 18 in docs/findings/borland-codegen.md,
 * which exists because of this function.
 *
 * Verdict RELOCATABLE.
 * Re-verified 2026-09-10 at the full 169-byte extent, trailing dead epilogue included: RELOCATABLE.
 *
 * RENAMED 2026-09-27 from pack_palette_rgb: Mac MSCopyRawColors(MSCTable*, unsigned char*, short, short). Callers: CTable::CopyRawColors and CTable::SetScale on both (Windows adds file writers). Body: packs palette entries into RGB triples. Arity 4. This also settles the const_proposal 1070:0000 palette_init -> MSCopyRawColors, rejected again below. PALETTE BLOCK: Windows 1070 palette_get_entry_rgb, palette_set_entry_rgb, palette_set_entry, [create_palette, palette_delete], palette_install_range, copy_logpalette, pack_palette_rgb. The Mac MS palette file has MSGetCTEntry, MSRawSetCTEntry, MSSetCTEntry, MSCopyRawColors, MSTagAnimate, MSTagNonAnimate (both bound), MSInitPalette, MSDestroyPalette, MSAnimatePal, MSUpdatePalette. The two pairs of equal-signature functions separate by caller and by the Win16 call in the body.
 */

#include "colonization.h"

#include "w16types.h"
struct logpal { unsigned short version; unsigned short count; PALETTEENTRY entries[1]; };

void MSCopyRawColors(struct logpal *pal, unsigned char *dst, int first, int count)
{
    int j = 0;
    int i;

    for (i = first; count + first > i; i++) {
        dst[j]     = pal->entries[i].peRed;
        dst[j + 1] = pal->entries[i].peGreen;
        dst[j + 2] = pal->entries[i].peBlue;
        j += 3;
    }
}
