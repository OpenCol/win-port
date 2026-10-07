/* draw_tile_with_overlays: candidate for 1040:131a (164 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Draws the 32-byte sprite record `kind` at the current tile position, then up to four
 * overlay records selected by a 4-byte list, skipping negative entries. Both record
 * tables are arrays of 32-byte entries; the position is a byte offset plus a word.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void draw_tile_with_overlays(unsigned char kind)
{
    signed char *p;
    char buf[8];
    int i;

    Sprite_Draw((RECT *)buf, &g_art_base[kind], &g_map_port, g_icon_dx + g_draw_px, g_icon_dy + g_draw_py);
    p = (signed char *)g_draw_pix;
    for (i = 0; i < 4; i++, p++) {
        if ((int)*p >= 0)
            Sprite_Draw((RECT *)buf, &g_overlay_sprites[*p], &g_map_port, g_icon_dx + g_draw_px, g_icon_dy + g_draw_py);
    }
}
