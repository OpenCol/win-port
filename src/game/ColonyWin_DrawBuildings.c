/* ColonyWin_DrawBuildings: candidate for 1010:8dec (264 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawBuildings
 * name-source: sibling-build
 *
 * Paints the fifteen building sites of the colony view (the 400x265 cell at
 * the origin): site i takes its building from SEG21:0x1dc, its position
 * from the pairs at SEG21:0x6a and its kind byte from SEG21:0x0; a built
 * site goes to ColonyWin_DrawABuilding, an empty one to ColonyWin_DrawATree.  With `redraw`
 * the cell is copied to the screen.  The two module helpers are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_draw_buildings: the Mac build's ColonyWin::DrawBuildings(short): this plus a redraw flag, walking the fifteen building sites.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int g_stub_a;
extern void set_colony(int colony);
extern void ColonyWin_UpdateColony(void *o, int x, int y, int w, int h);
extern void ColonyWin_DrawABuilding(void *w, int b, int x, int y, int kind);

extern "C" void ColonyWin_DrawATree(void *w, int x, int y, int kind);
extern "C" void copy_rect_to_screen_1010_e6a4(void *w, RECT *r);

void ColonyWin_DrawBuildings(ColonyWin *w, int redraw)
{
    register int i;
    register int building;
    int x;
    int y;
    int kind;
    RECT r;

    set_colony(w->colony);
    ColonyWin_UpdateColony(w, 0, 0, 0x190, 0x109);
    for (i = 0; i < 15; i++) {
        building = g_site_building[i];
        x = g_site_pos[i].x;
        y = g_site_pos[i].y;
        kind = g_site_kind[i];
        if (building >= 0)
            ColonyWin_DrawABuilding(w, building, x, y, kind);
        else
            ColonyWin_DrawATree(w, x, y, kind);
    }
    if (redraw != 0) {
        SetRect(&r, w->rect.left, w->rect.top, w->rect.left + 0x190, w->rect.top + 0x109);
        copy_rect_to_screen_1010_e6a4(w, &r);
    }
}
