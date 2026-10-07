/* MapWin_DrawCursor: candidate for 1038:aae0 (166 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_DrawCursor
 * name-source: sibling-build
 *
 * Draws the tile cursor sprite (SEG20:0x64a6) at map tile (x, y): tile
 * size from MapWin_TileSize, the view origin at DGROUP:0x4c44/0x4c46, the
 * sprite scale set to (zoom + 8) by 8 through SetGlobalSpriteScale and back
 * to 1 by 1.  The two MapWin helpers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from map_draw_cursor: the Mac build's MapWin::DrawCursor(short, short): this plus a map tile, drawing the tile cursor there. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MapWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "icons.h"

extern int g_stub_a;
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern "C" int MapWin_TileSize(MapWin *w);
extern "C" int MapWin_GetZoom(MapWin *mw);

void MapWin_DrawCursor(MapWin *w, int x, int y)
{
    register int ts;
    int dx;
    int dy;
    int bank;
    RECT r;

    ts = MapWin_TileSize(w);
    dx = (x - g_view_x0) * ts;
    dy = (y - g_view_y0) * ts;
    bank = MapWin_GetZoom(w) + 8;
    SetGlobalSpriteScale(bank, 8);
    Sprite_Draw(&r, &g_icon_sprites[ICON_CURSOR], (GRPort *)w, w->rect.left + dx, w->rect.top + dy);
    SetGlobalSpriteScale(1, 1);
}
