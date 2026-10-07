/* PORT: this file's own declarations of GenerateUnit removed: the header declares it from the definition (W1 step 3). */
/* GenerateUnitsRegion: candidate for 1040:0f93 (327 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws every chain-head unit (link +0x18 negative) inside the map rectangle, clipped to the
 * view: the player's own units when the square has been seen (plane-3 bit), others when the
 * unit's own flag byte carries the player's bit 0x10 << nation -- or always with reveal on.
 *
 * RENAMED 2026-09-26 from draw_units_in_rect: the Mac build's GenerateUnitsRegion(short, short, short, short), bound by call-sequence alignment (2.0 aligned gaps in GameBrains_CursorUpdate, DrawMapRegion, MapWin_SlideIcon leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.96x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void ConformToView(int *l, int *top, int *r, int *bottom);
extern unsigned char site_get(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
void GenerateUnitsRegion(int x0, int y0, int w, int h)
{
    int i;
    int ux;
    int uy;
    int y0c;
    int x0c;
    int x1;
    int y1;
    int vis;
    unsigned char bit;

    x0c = x0;
    x1 = x0 + w - 1;
    y0c = y0;
    y1 = y0 + h - 1;
    ConformToView(&x0c, &y0c, &x1, &y1);
    bit = (unsigned char)((1 << ((unsigned char)g_game.view_nation)) << 4);
    for (i = 0; g_game.unit_count > i; i++) {
        if (g_units[i].link < 0) {
            ux = g_units[i].x;
            uy = g_units[i].y;
            if (ux >= x0c && ux <= x1 && uy >= y0c && uy <= y1) {
                if ((unsigned char)(g_units[i].flags & UNIT_NATION_MASK) == ((unsigned char)g_game.view_nation))
                    vis = site_get(ux, uy) & bit;
                else
                    vis = g_units[i].flags & UNIT_SEEN_BY(((unsigned char)g_game.view_nation));
                if (vis != 0 || g_game.reveal_all != 0)
                    GenerateUnit(i);
            }
        }
    }
}
