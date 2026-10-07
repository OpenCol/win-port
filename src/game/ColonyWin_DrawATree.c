/* ColonyWin_DrawATree: candidate for 1010:8d3e (174 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawATree
 * name-source: sibling-build
 *
 * The empty half of `ColonyWin_DrawBuildings`'s pair: that function walks the
 * fifteen building sites of the colony view and sends a built one to `ColonyWin_DrawABuilding` and an
 * unbuilt one here. What an unbuilt site draws is the GROUND under it -- the sprite id in
 * the word table at SEG21:0x60, indexed by the site's kind byte, biased by 0x15e -- and a
 * kind whose entry is zero draws nothing at all.
 *
 * The position is the caller's offset plus the child window's own origin, which is why the
 * two additions are on the arguments rather than in the caller.
 *
 * The `Sprite` is a stack object with a destructor, so the function carries an exception
 * frame and the destructor runs on both paths.
 *
 * RENAMED 2026-09-26 from draw_empty_site: the Mac build's ColonyWin::DrawATree(short, short, short): this plus three shorts, painting the ground sprite of an unbuilt site -- the other half of ColonyWin_DrawBuildings' built/unbuilt pair with ColonyWin_DrawABuilding.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern void set_colony(int colony);
extern void Sprite_Init(Sprite *s, int res);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void ColonyWin_DrawATree(ColonyWin *w, int x, int y, int kind)
{
    Sprite s;
    RECT r;
    register int id;

    set_colony(w->colony);
    id = g_site_ground[kind];
    if (id != 0) {
        Sprite_Init(&s, id + 0x15e);
        Sprite_Draw(&r, &s, w, w->rect.left + x, w->rect.top + y);
    }
}

