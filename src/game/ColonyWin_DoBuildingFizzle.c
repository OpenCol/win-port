/* PORT: this file's own declarations of Sprite_Init removed: the header declares it from the definition (W1 step 3). */
/* ColonyWin_DoBuildingFizzle: candidate for 1010:a28a (571 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DoBuildingFizzle
 * name-source: sibling-build
 *
 * Announces a building the colony has just finished. The building's index waits at
 * DGROUP:0xc20 -- `produce_something` puts it there, minus the 0x1f case -- and -1 means
 * there is nothing to announce, which is the only early exit. The word is put back to -1 on
 * the way out, so the announcement happens once.
 *
 * The site is found by walking the fifteen colony sites until one's building matches; the
 * walk leaves the site's position in `px`/`py` and its building in `b`, and a walk that
 * finds nothing leaves the fifteenth site's.
 *
 * WHICH SPRITE IS DRAWN IS NOT ALWAYS `b + 1`. Building 0 with the colony's flag 0 clear
 * draws sprite 0 instead -- the empty lot. Buildings 0xf and 0x11 are the same lot at two
 * levels: with flag 0xf set the sprite is 0x11 only when flag 0x11 is set too, and with flag
 * 0xf clear it is 0x12 whatever `b` said. Sprite 0x15e is the base of the run.
 *
 * The sprite is drawn into a scratch GRPort sized to itself and filled with DGROUP:0x337e
 * first, and `wipe_rows` moves it onto the colony window at the site's offset -- the
 * scratch port is what lets the transition read both surfaces.
 *
 * Two local objects with destructors, so the function carries Borland's exception block and
 * both destructors run on both exits. All four constructor and destructor calls are FAR here,
 * so the classes are declared and not defined (rule 47).
 *
 * RENAMED 2026-09-26 from colony_flash_building: the Mac build's ColonyWin::DoBuildingFizzle(void): this only, announcing the building the colony has just finished with its flashing effect. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "building.h"
#include "classes.h"
#include "records.h"

extern void set_colony(int colony);
extern int has_building(int bit);

extern int GRPort_Init(GRPort *p, void *r);
extern void GRPort_FillColor(GRPort *p, RECT *r, int colour);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void wipe_rows(GameWin *win_a, GameWin *win_b, RECT *src_rect, int x, int y, int mode, int step, int delay);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int sprite_width_1010_e60f(Sprite *sprite);
extern "C" int sprite_height_1010_e681(Sprite *sprite);

void ColonyWin_DoBuildingFizzle(ColonyWin *w)
{
    GRPort port;
    Sprite s;
    RECT r;
    int idx;
    int px;
    int py;
    RECT tmp;
    register int i;
    register int building;

    set_colony(w->colony);
    g_draw_labels = g_show_icon_counts;
    if (g_new_building == -1)
        return;
    for (i = 0; i < 0xf; i++) {
        building = g_site_building[i];
        px = g_site_pos[i].x;
        py = g_site_pos[i].y;
        if (g_new_building == building)
            break;
    }
    idx = building + 1;
    if (g_new_building == 0 && has_building(BUILDING_STOCKADE) == 0)
        idx = 0;
    if (building == 0xf || building == 0x11) {
        if (has_building(BUILDING_WAREHOUSE) != 0) {
            if (has_building(BUILDING_STABLES) != 0)
                idx = 0x11;
        } else {
            idx = 0x12;
        }
    }
    Sprite_Init(&s, idx + 0x15e);
    SetRect(&r, 0, 0, sprite_width_1010_e60f(&s), sprite_height_1010_e681(&s));
    GRPort_Init(&port, &r);
    GRPort_FillColor(&port, &r, g_draw_color);
    Sprite_Draw(&tmp, &s, &port, 0, 0);
    wipe_rows((GameWin *)&port, w, &r, w->rect.left + px, w->rect.top + py, g_draw_color, 3, 1);
    g_new_building = -1;
}

