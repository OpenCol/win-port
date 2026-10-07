/* wing_blit: candidate for 1078:26d1 (100 bytes).  Verdict: RELOCATABLE.
 *
 * Copies a w x h block from a WinG surface's DC to a window's DC with WinGBitBlt, realising the
 * window's palette first when it has one and the palette mode (DGROUP:0x3380) is 1; nothing when
 * either side is null.
 */

#include "colonization.h"

#include "records.h"

void wing_blit(GrafRec *src, int sx, int sy, int w, int h, MSWin *dst, int dx, int dy)
{
    if (src != 0 && dst != 0) {
        if (dst->hpal != 0 && g_palette_mode == 1)
            RealizePalette(dst->hdc);
        WinGBitBlt(dst->hdc, dx, dy, w, h, src->hdc, sx, sy);
    }
}
