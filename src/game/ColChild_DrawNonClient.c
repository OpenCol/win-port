/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of GRPort_GetClip removed: the header declares it from the definition (W1 step 3). */
/* ColChild_DrawNonClient: candidate for 1048:077f (700 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DrawNonClient
 * name-source: sibling-build
 *
 * Paints a window's own frame: the wooden border tiled from sprite 411 of the sheet at SEG20:0x41a6 (g_background_tile, 0x7506), a raised bevel round
 * the whole port, a sunken one round the client rectangle (+0x76a) grown by two, an outline, and the title in
 * colour 0xc across the middle.
 *
 * THE BORDER IS TILED AS TWO STRIPS AND NOT AS A RECTANGLE. The first loop lays the LEFT and
 * RIGHT columns, three tiles per row -- x = 0, x = the snapped width, and one tile inside it --
 * and the second lays the TOP and BOTTOM rows the same way. The middle is never touched, which
 * is what makes this a frame rather than a background.
 *
 * The snapped width is `cw / tile_w * tile_w`, integer division doing the work: the last full
 * tile that fits. The third tile in each strip is drawn one tile back from it, so a window
 * whose width is not a multiple of the tile still closes cleanly at the corner.
 *
 * Both loops run to `size / tile + 1`, one past the last full tile, so the edge is always
 * covered even when the division truncates.
 *
 * The two bevels differ only in the SIGN of the depth -- §297's `FrameInfoRect` reads
 * that as raised against sunken -- so the window stands out and the client area sinks in.
 *
 * `col` is the register local in SI and `row` in DI, which is declaration order and NOT the
 * order the loops run in: the row loop comes first and uses DI (rule 53).
 *
 * `g_art_base[411]` IS `g_background_tile`. SEG20:0x41a6 is the base of the whole 32-byte sprite
 * table, and six globals that look independent are windows into it, each an exact multiple of
 * 0x20 from that base:
 *
 *   index  28  SEG20:0x4526  folded away here; it was `g_sprites_4526`
 *   index 217  SEG20:0x5cc6  g_overlay_sprites
 *   index 261  SEG20:0x6246  g_icon_sprites
 *   index 284  SEG20:0x6526  g_goods_sprites
 *   index 411  SEG20:0x7506  g_background_tile
 *   index 413  SEG20:0x7546  g_edge_sprites
 *
 * So this function tiles a window's frame with the background tile, which the index says and the
 * name confirms. The table is rooted at 0x41a6 and not at one of its windows because a NEGATIVE
 * constant index does not fold into a relocation -- `[-28]` compiles to arithmetic at run time --
 * which `docs/reconstruction/next-session.md` records from the session that found it.
 *
 * RENAMED 2026-09-26 from draw_window_frame: the Mac build's ColChild::DrawNonClient(void): this only, painting the window's own frame -- border, bevels and title -- the non-client area. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, Font, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void GRPort_ResetClip(GRPort *port);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void FrameInfoRect(GRPort *w, RECT *r, int depth, int light, int dark);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern void GRPort_SetClip(GRPort *port, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void ColChild_DrawNonClient(ColChild *w)
{
    int cw;
    int ch;
    int gx;
    int gy;
    RECT r;
    RECT clip;
    RECT t1;
    RECT t2;
    RECT t3;
    RECT t4;
    RECT t5;
    RECT t6;
    register int col;
    register int row;

    GRPort_GetClip(w, &clip);
    GRPort_ResetClip((GRPort *)w);
    cw = port_width_1048_d4f6(w);
    ch = port_height_1048_d7c5(w);
    gx = cw / w->tile_w * w->tile_w;
    gy = ch / w->tile_h * w->tile_h;
    SetGlobalSpriteScale(1, 1);
    for (row = 0; ch / w->tile_h + 1 > row; row++) {
        Sprite_Draw(&t1, &g_background_tile, (GRPort *)w, 0, w->tile_h * row);
        Sprite_Draw(&t2, &g_background_tile, (GRPort *)w, gx, w->tile_h * row);
        Sprite_Draw(&t3, &g_background_tile, (GRPort *)w, gx - w->tile_w, w->tile_h * row);
    }
    for (col = 0; cw / w->tile_w + 1 > col; col++) {
        Sprite_Draw(&t4, &g_background_tile, (GRPort *)w, w->tile_w * col, 0);
        Sprite_Draw(&t5, &g_background_tile, (GRPort *)w, w->tile_w * col, gy);
        Sprite_Draw(&t6, &g_background_tile, (GRPort *)w, w->tile_w * col, gy - w->tile_h);
    }
    SetRect(&r, 0, 0, cw, ch);
    FrameInfoRect((GRPort *)w, &r, 2, 0x16, 0x12);
    SetRect(&r, w->rect.left - 2, w->rect.top - 2, w->rect.right + 2, w->rect.bottom + 2);
    FrameInfoRect((GRPort *)w, &r, -2, 0xff, 0xf8);
    InflateRect(&r, 1, 1);
    TextColor(0);
    GRPort_DrawRect((GRPort *)w, &r);
    TextColor(0xc);
    GRPort_String((GRPort *)w, (unsigned *)&g_title_font, w->title, cw / 2, 0x12, 0x10);
    TextColor(0);
    GRPort_SetClip((GRPort *)w, &clip);
}
