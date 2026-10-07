/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of GRPort_GetClip, copy_rect_to_screen removed: the header declares them from the definition (W1 step 3). */
/* ColChild_DrawTitleBar: candidate for 1048:0ecf (394 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DrawTitleBar
 * name-source: sibling-build
 *
 * Draws a window's title bar, when the window has both a height and a width: the port is
 * clipped to the strip from (2, 2) to 0x1e down and two short of the right edge, the tile sprite
 * at SEG20:0x7506 is laid across it in two rows (width / tile width + 1 columns, each tile
 * w->tile_w by w->tile_h), and the title at +0xc4 is drawn centred at y 0x12 in colour 0xc with
 * the font at DGROUP:0x4b06. Then the port's rectangle and clip are put back, the strip copied to
 * the screen, and the map scale that `GetGlobalSpriteScale` saved restored after the tiles
 * were drawn at 1:1.
 *
 * THE TILE POSITIONS ARE `long` PRODUCTS truncated to the `int` arguments (rule 65): `MOV AX,DI /
 * LES BX / IMUL WORD [ES:BX+0x146]`, the counter loaded first. Every `int` spelling of the product
 * -- either operand order, casts on either side, `register` or promoted counters, `(*w).` or
 * `w[0].` -- loads the member into AX and multiplies by the register instead.
 *
 * RENAMED 2026-09-26 from draw_window_title: the Mac build's ColChild::DrawTitleBar(void): this only, painting the title bar strip. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, Font, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GetGlobalSpriteScale(int *num, int *den);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_ResetClip(GRPort *port);

/* same object: 4-byte `push cs; call ` (rule 47) */


void ColChild_DrawTitleBar(ColChild *w)
{
    RECT r;
    RECT clip;
    int sx;
    int sy;
    int n;
    RECT out;
    int j;
    int i;

    if (port_height_1048_d7c5(w) != 0 && port_width_1048_d4f6(w) != 0) {
        GRPort_GetClip(w, &clip);
        SetRect(&r, 2, 2, port_width_1048_d4f6(w) - 2, 0x1e);
        GRPort_SetClip((GRPort *)w, &r);
        GetGlobalSpriteScale(&sx, &sy);
        SetGlobalSpriteScale(1, 1);
        n = port_width_1048_d4f6(w) / w->tile_w + 1;
        for (j = 0; j < 2; j++)
            for (i = 0; i < n; i++)
                Sprite_Draw(&out, &g_background_tile, (GRPort *)w, (long)i * w->tile_w, (long)j * w->tile_h);
        TextColor(0xc);
        GRPort_String((GRPort *)w, (unsigned *)&g_title_font, w->title, port_width_1048_d4f6(w) / 2, 0x12, 0x10);
        TextColor(0);
        GRPort_ResetClip((GRPort *)w);
        GRPort_SetClip((GRPort *)w, &clip);
        copy_rect_to_screen(w, &r);
        SetGlobalSpriteScale(sx, sy);
    }
}
