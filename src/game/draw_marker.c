/* draw_marker: candidate for 1038:a849 (342 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: draw_marker
 * name-source: descriptive
 *
 * Draws a sprite on the map with an optional two-pixel SHADOW on either side. Bit 3 puts a
 * copy two pixels right, bit 0 a copy two pixels left, and bit 1 the sprite itself on top --
 * so the three bits are independent and a caller can ask for shadow without substance.
 *
 * Bit 2 chooses the shadow's colour: 0x5f when set, otherwise the byte at DGROUP:0x337e that
 * `CDControlWin_DrawTrack` also fills with.
 *
 * The map scale is set to `(w->zoom + 8) / 8` for the drawing and put back to 1/1 afterwards,
 * which is what makes the same sprite come out at the window's current zoom.
 *
 * +0x76a IS THE CLIP RECTANGLE, and its first two words are what other reconstructions in this
 * corpus call the window's `ox`/`oy`: the clip is passed WHOLE to `GRPort_SetClip` and
 * its corner is what every coordinate here is measured from. One field, two readings, and the
 * displacement byte tells them apart.
 *
 * `Sprite_DrawMono`'s last parameter is an `unsigned char`: the shipped sets AL in both arms of
 * the ternary where an `int` parameter gives `MOV AX,0x5F` and a widening of the global. That
 * corrects `ReportWin_ReportsLabor`, which passes a literal 0 there and so could not tell.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MapWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void SetGlobalSpriteScale(int num, int den);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *port, int x, int y, unsigned char mode);

void draw_marker(MapWin *w, register int spr, int x, int y, register int flags)
{
    int scale;
    int base;
    RECT r1;
    RECT r2;
    RECT r3;

    scale = w->zoom + 8;
    base = 8;
    SetGlobalSpriteScale(scale, base);
    GRPort_SetClip((GRPort *)w, &w->rect);
    if (flags & 8)
        Sprite_DrawMono(&r1, &g_icon_sprites[spr], (GRPort *)w, w->rect.left + x + 2, w->rect.top + y,
                     (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 1)
        Sprite_DrawMono(&r2, &g_icon_sprites[spr], (GRPort *)w, w->rect.left + x - 2, w->rect.top + y,
                     (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        Sprite_Draw(&r3, &g_icon_sprites[spr], (GRPort *)w, w->rect.left + x, w->rect.top + y);
    GRPort_ResetClip((GRPort *)w);
    SetGlobalSpriteScale(1, 1);
}
