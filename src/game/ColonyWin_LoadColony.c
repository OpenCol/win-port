/* ColonyWin_LoadColony: candidate for 1010:bb30 (527 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_LoadColony
 * name-source: sibling-build
 *
 * Paints the colony window's backdrop into the saved GRPort at +0x7a8, which everything else
 * in the window then blits out of (`ColonyWin_UpdateColony`, the next function, is the
 * blitter).
 *
 * TWO TILINGS SIDE BY SIDE. The left 0x190 columns get sprite 384 on a flat 0x20 grid down to
 * y = 0x138; the right-hand strip from 0x190 to 0x280 gets sprite 383 on a 0x3e by 0x2f grid
 * down to y = 0x149 -- the same sprite and the same spacing `PopupBoxDraw`
 * uses for its own background, so the two screens are papered alike.
 *
 * Four rectangles in colour 0 frame the two halves, each drawn twice one pixel in, which is
 * what makes the border two pixels thick. Then picture resource 0xfa, 0x28 by 0x60, is loaded
 * into a local port and blitted in at y = 0x109 -- directly under the two framed halves, which
 * are 0x109 tall.
 *
 * The local port has a destructor, so the function carries Borland's exception block; the two
 * size accessors come through the four-byte `PUSH CS` form and are stubs (rule 47).
 *
 * RENAMED 2026-09-26 from colony_paint_back: the Mac build's ColonyWin::LoadColony(void): this only; ColonyWin_Show calls it once to paint the window's backdrop port (+0x7a8) that every panel then blits from -- EuropeWin_LoadEurope's counterpart. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, CTable, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x41e bytes: the palette, its handle and the cycling state */

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_DrawRect(GRPort *p, RECT *r);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int port_width_1010_ebd4(GRPort *port);
extern "C" int port_height_1010_ebed(GRPort *port);

void ColonyWin_LoadColony(ColonyWin *w)
{
    GRPort pic;
    RECT r;
    RECT r2;
    RECT t1;
    RECT t2;
    register int x;
    register int y;

    GRPort_Init_wh(&w->back, 0x280, 0x1b8);
    for (y = 0; y < 0x138; y += 0x20)
        for (x = 0; x < 0x190; x += 0x20)
            Sprite_Draw(&t1, &g_route_panel_sprite, &w->back, x, y);
    for (y = 0; y < 0x149; y += 0x2f)
        for (x = 0x190; x < 0x280; x += 0x3e)
            Sprite_Draw(&t2, &g_background_tile, &w->back, x, y);
    TextColor(0);
    SetRect(&r, 0, 0, 0x190, 0x109);
    GRPort_DrawRect(&w->back, &r);
    SetRect(&r, 1, 1, 0x18f, 0x108);
    GRPort_DrawRect(&w->back, &r);
    SetRect(&r, 0x190, 0, 0x280, 0x109);
    GRPort_DrawRect(&w->back, &r);
    SetRect(&r, 0x191, 1, 0x27f, 0x108);
    GRPort_DrawRect(&w->back, &r);
    GRPort_LoadPicture(&pic, 0xfa, 0x28, 0x60, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1010_ebd4(&pic), port_height_1010_ebed(&pic));
    CopyRect(&r2, &r);
    OffsetRect(&r2, 0, 0x109);
    GRPort_CopyToPort(&pic, &w->back, &r, &r2);
}

