/* PORT: this file's own declarations of GRPort_GetWin_1038_e355, copy_rect_to_screen_1038_e38f, get_win_1038_e372, wing_curtain_open_h_at removed: the header declares them from the definition (W1 step 3). */
/* MapWin_RevealHiddenTerrain: candidate for 1038:a441 (306 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_RevealHiddenTerrain
 *
 * Redraws the whole map with the map mode at DGROUP:0x1ca4 set to 1
 * (hidden terrain shown): the view relaid into the backdrop, then a
 * horizontal curtain transition from the backdrop port (SEG28:0) onto the
 * window's DialogWin, the backdrop copied and blitted, the mode reset.
 * Five helpers are -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_stub_a;
extern void GRPort_SetClip(GRPort *port, RECT *r);

extern void GenerateTerrainMap(int nation);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void GRPort_ResetClip(GRPort *port);


extern "C"   /* PORT: C linkage: C files call it too (W2) */
void MapWin_RevealHiddenTerrain(MapWin *w)
{
    RECT r;

    g_map_mode = 1;
    GRPort_SetClip((GRPort *)w, &w->rect);
    SetRect(&r, 0, 0, rect_width_1038_dfbd(&w->rect), rect_height_1038_dfdc(&w->rect));
    GenerateTerrainMap(g_game.reveal_all != 0 ? -1 : g_game.view_nation);
    wing_curtain_open_h_at((GrafRec *)(GRPort_GetWin_1038_e355(w)), (GrafRec *)(GRPort_GetWin_1038_e355(&g_map_port)), (MSWin *)(get_win_1038_e372(&(DialogWin &)*w)), &r, w->rect.left, w->rect.top, 8);
    GRPort_CopyToPort(&g_map_port, (GRPort *)w, &r, &w->rect);
    copy_rect_to_screen_1038_e38f(w, &w->rect);
    GRPort_ResetClip((GRPort *)w);
    g_map_mode = 0;
}
