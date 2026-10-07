/* DrawColoniesRegion: candidate for 1040:09e2 (333 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws every colony inside the map rectangle (clipped to the view) that can_see
 * admits for the player and that the player has seen (plane-3 bit) -- or all with reveal on:
 * pixel position from the view origin and tile size, sprite size from the zoom.
 *
 * THE DRAW TARGET IS `g_map_port`, SETTLED 2026-09-28 from the shipped bytes. This was
 * declared `g_colony_sprites` with no offset, which named it a sprite table; it is not one --
 * it is the first argument of `FortDraw`, which every other caller gives a window
 * (`ReportWin_ReportsColony` passes `o`, `StatWin_Stats` `w`, `combat_analysis` `g_win`).
 * Segment 28 tiles exactly as two GRPorts and two Sprites (`g_map_port` +0, 0x26 bytes;
 * Sprites at +0x26 and +0x46; `g_icon_port` +0x66), and the OFFSET16 the shipped code
 * loads at 1040:0b0d is 0x0000 in SEG28 (tools/check_data_refs.py), so of the two ports
 * it is the one at +0.
 *
 * RENAMED 2026-09-27 from draw_colonies_in_rect: NOT PROPOSED BY mac_bind. Mac DrawColoniesRegion(short, short, short, short). Caller: DrawColonies(void) (draw_colonies_view). Callees: ConformToView, FortDraw, MapWin::GetZoom, can_see and site_get, 5 of 5. Arity: 4. No overload. Layout: Mac CODE 22 ShowMap, ShowMapRegion, DrawVillagesRegion, DrawVillages(void), DrawColoniesRegion, DrawColonies(void), GenerateUnitStack; Windows 1040 ShowMap, ShowMapRegion, 0877, draw_settlements_view, 09e2, draw_colonies_view, stack_icon (Windows-only), GenerateUnitStack.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void ConformToView(int *l, int *top, int *r, int *bottom);
extern int can_see(int colony, int nation);
extern unsigned char site_get(int x, int y);
extern int MapWin_GetZoom(void *mw);
extern void FortDraw(GRPort *sprites, int i, int px, int py, int size, int a, int b);

void DrawColoniesRegion(int x0, int y0, int w, int h)
{
    int i;
    int cx;
    int cy;
    int px;
    int py;
    int x1;
    int y1;
    unsigned char bit;

    bit = (unsigned char)((1 << g_game.view_nation) << 4);
    x1 = x0 + w - 1;
    y1 = y0 + h - 1;
    ConformToView(&x0, &y0, &x1, &y1);
    for (i = 0; g_game.colony_count > i; i++) {
        cx = g_colonies[i].x;
        cy = g_colonies[i].y;
        if (cx >= x0 && cx <= x1 && cy >= y0 && cy <= y1) {
            if (can_see(i, g_game.view_nation) != 0) {
                if ((site_get(cx, cy) & bit) || g_game.reveal_all != 0) {
                    px = (cx - g_view_x0) * g_tile_w;
                    py = (cy - g_view_y0) * g_tile_h;
                    FortDraw(&g_map_port, i, px, py, MapWin_GetZoom(&g_map_window) + 8, 1, 1);
                }
            }
        }
    }
}
