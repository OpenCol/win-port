/* DrawVillagesRegion: candidate for 1040:0877 (302 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws every native settlement inside the map rectangle (x0, y0, w, h), clipped to the
 * view, that the player has seen (plane-3 bit 4 + nation) or when reveal (SEG20:0x77a4) is
 * on: pixel position from the view origin and tile size, sprite size from the zoom.
 *
 * THE DRAW TARGET IS `g_map_port`, SETTLED 2026-09-28 from the shipped bytes. This was
 * declared `g_settlement_sprites` with no offset, which named it a sprite table; it is not one --
 * it is the first argument of `TeepeeDraw`, which every other caller gives a window
 * (`ReportWin_ReportsColony` passes `o`, `StatWin_Stats` `w`, `combat_analysis` `g_win`).
 * Segment 28 tiles exactly as two GRPorts and two Sprites (`g_map_port` +0, 0x26 bytes;
 * Sprites at +0x26 and +0x46; `g_icon_port` +0x66), and the OFFSET16 the shipped code
 * loads at 1040:0983 is 0x0000 in SEG28 (tools/check_data_refs.py), so of the two ports
 * it is the one at +0.
 *
 * RENAMED 2026-09-27 from draw_settlements_in_rect: NOT PROPOSED BY mac_bind. Mac DrawVillagesRegion(short, short, short, short). Caller: DrawVillages(void) (draw_settlements_view) on both. Callees: ConformToView, MapWin::GetZoom, TeepeeDraw and site_get, 4 of 4. Arity: 4 = (x0, y0, w, h). No overload. Layout: Mac CODE 22 ShowMap, ShowMapRegion, DrawVillagesRegion, DrawVillages(void), DrawColoniesRegion, DrawColonies(void), GenerateUnitStack; Windows 1040 ShowMap, ShowMapRegion, 0877, draw_settlements_view, 09e2, draw_colonies_view, stack_icon (Windows-only), GenerateUnitStack.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void ConformToView(int *l, int *top, int *r, int *bottom);
extern unsigned char site_get(int x, int y);
extern int MapWin_GetZoom(void *mw);
extern void TeepeeDraw(void *sprites, int i, int px, int py, int size);

void DrawVillagesRegion(int x0, int y0, int w, int h)
{
    int i;
    int sx;
    int sy;
    int px;
    int py;
    int x1;
    int y1;
    unsigned char bit;

    bit = (unsigned char)((1 << ((unsigned char)g_game.view_nation)) << 4);
    x1 = x0 + w - 1;
    y1 = y0 + h - 1;
    ConformToView(&x0, &y0, &x1, &y1);
    for (i = 0; g_game.settlement_count > i; i++) {
        sx = g_settlements[i].x;
        sy = g_settlements[i].y;
        if (sx >= x0 && sx <= x1 && sy >= y0 && sy <= y1) {
            if ((site_get(sx, sy) & bit) || g_game.reveal_all != 0) {
                px = (sx - g_view_x0) * g_tile_w;
                py = (sy - g_view_y0) * g_tile_h;
                TeepeeDraw(&g_map_port, i, px, py, MapWin_GetZoom(&g_map_window) + 8);
            }
        }
    }
}
