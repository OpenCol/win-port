/* palette_delete: candidate for 1070:0471 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Deletes a GDI palette handle, but only when the palette mode word at DGROUP:0x3380 is 1 --
 * the same guard `MSUpdatePalette` (1070:048e, immediately after this) puts on
 * SetPaletteEntries. In any other mode there is no GDI palette to free.
 *
 * Its one caller is `CTable_dtor`, which passes the colour table's word at +0x404: the field
 * `MSUpdatePalette`'s record already names `hpal`.
 */

#include "colonization.h"

void palette_delete(unsigned hpal)
{
    if (g_palette_mode == 1)
        DeleteObject(hpal);
}
