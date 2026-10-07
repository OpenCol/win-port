/* wing_stretch: candidate for 1078:2735 (106 bytes).  Verdict: RELOCATABLE.
 *
 * Stretches a source block from a port's WinG surface (GrafRec, its DC at +2) to a window's DC
 * with WinGStretchBlt, realising the window's palette first when it has one and the palette mode (DGROUP:0x3380) is 1; nothing when
 * either side is null.
 */

#include "colonization.h"

#include "records.h"

void wing_stretch(GrafRec *src, int sx, int sy, int sw, int sh, MSWin *dst, int dx, int dy, int dw, int dh)
{
    if (src != 0 && dst != 0) {
        if (dst->hpal != 0 && g_palette_mode == 1)
            RealizePalette(dst->hdc);
        WinGStretchBlt(dst->hdc, dx, dy, dw, dh, src->hdc, sx, sy, sw, sh);
    }
}
