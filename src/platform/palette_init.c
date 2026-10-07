/* palette_init: candidate for 1070:0000 (454 bytes).  Verdict: RELOCATABLE.
 * symbol: palette_init
 * name-source: descriptive
 *
 * Builds the game's LOGPALETTE. A display without RC_PALETTE (bit 8 of RASTERCAPS) gets no
 * system entries at all and both DGROUP:0x56c2 and 0x3380 are cleared; a palette display has
 * its whole system palette read in, and HALF of `GetDeviceCaps(NUMCOLORS)` stored at 0x56c2 as
 * the reserved count. On a palette device NUMCOLORS is the number of STATIC colours -- twenty
 * -- so the count is ten, one end's share.
 *
 * THREE LOOPS SHARE ONE COUNTER AND NONE OF THEM RESTARTS IT. The first clears the flags over
 * the system's bottom entries, the second blacks out everything up to `0x100 - reserved` and
 * flags it PC_NOCOLLAPSE, and the third clears the flags again over the top entries. So the
 * middle band is the game's, and the reserved count is subtracted from BOTH ends -- the bottom
 * by starting there and the top by the `0x100 - reserved` bound -- which is why the static
 * count is halved before it is stored. PC_NOCOLLAPSE keeps Windows from folding the game's
 * entries onto matching system colours, so every one of them gets its own slot.
 *
 * (This header first read 0x18 as SIZEPALETTE and 4 as PC_RESERVED. WINDOWS.H has them as
 * NUMCOLORS and PC_NOCOLLAPSE; include/w16const.h is generated from it, and naming the two
 * constants is what showed the error. RE_STATUS 304 and 342.)
 *
 * Ten entries at the bottom and ten at the top are then overwritten from a template at
 * DGROUP:0x3382, but only when 0x3380 is zero: the twenty Windows system colours, put back so
 * the game's palette does not fight the desktop's.
 *
 * The template pointer is a plain `` walked with `*src++` on the offset alone, and it reads
 * sixty bytes, so it cannot cross a segment.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"

#include "records.h"

void palette_init(Palette *pal)
{
    int caps;
    unsigned dc;
    unsigned char *src;
    register int i;

    pal->version = 0x300;
    pal->count = 0x100;
    dc = GetDC(0);
    caps = GetDeviceCaps(dc, RASTERCAPS);
    if ((caps & RC_PALETTE) == 0) {
        g_reserved_count = 0;
        g_palette_mode = 0;
    } else {
        g_reserved_count = GetDeviceCaps(dc, NUMCOLORS);
        g_reserved_count = g_reserved_count / 2;
        GetSystemPaletteEntries(dc, 0, 0x100, pal->e);
    }
    for (i = 0; i < g_reserved_count; i++)
        pal->e[i].reserved = 0;
    for (; 0x100 - g_reserved_count > i; i++) {
        pal->e[i].r = 0;
        pal->e[i].g = 0;
        pal->e[i].b = 0;
        pal->e[i].reserved = PC_NOCOLLAPSE;
    }
    for (; i < 0x100; i++)
        pal->e[i].reserved = 0;
    if (g_palette_mode == 0) {
        src = g_palette_template;
        for (i = 0; i < 0xa; i++) {
            pal->e[i].r = *src++;
            pal->e[i].g = *src++;
            pal->e[i].b = *src++;
        }
        for (i = 0xf6; i < 0x100; i++) {
            pal->e[i].r = *src++;
            pal->e[i].g = *src++;
            pal->e[i].b = *src++;
        }
    }
    ReleaseDC(0, dc);
}
