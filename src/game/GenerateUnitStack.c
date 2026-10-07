/* GenerateUnitStack: candidate for 1040:0d04 (298 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws one unit on the map, and the two flags it takes decide whether it draws at all. The
 * pair `(a, b)` is `(1, 0)` for the arrived unit's normal frame, `(1, 1)` for its blink and
 * `(0, 0)` for everything else -- `GenerateActiveUnit` and `GenerateUnit`
 * are the only callers and pass only those three.
 *
 * `(1, 0)` returns at once. Otherwise, when either flag is clear, a unit STANDING IN A COLONY
 * is skipped as well: the colony's own sprite covers it.
 *
 * The style word starts at 0x80 or 0xc0 by `a`, and 0x20 is added for any unit not of the
 * viewing nation -- so the third bit of that word is "someone else's".
 *
 * The pixel position is the plain tile transform, `(tile - origin) * size`, with HALF A TILE
 * added on the x axis only: units are centred across their square and hang from its top.
 *
 * `SAR AX,1` with no rounding correction is `>> 1` and not `/ 2`.
 *
 * `PieceDraw_noport` is reached through the five-byte NOP form and is `extern` (rule 47).
 *
 * RENAMED 2026-09-26 from paint_unit: the Mac build's GenerateUnitStack(short, short, short), bound by call-sequence alignment (2.0 aligned gaps in GenerateActiveUnit, GenerateUnit leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.90x this one.
 */

#include "colonization.h"

#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern void PieceDraw_noport(int u, int flags, int x, int y, int w, int h);

void GenerateUnitStack(register int u, register int active, int blink)
{
    int px;
    int py;
    int x;
    int y;
    int col;
    int flags;

    if (active != 0 && blink == 0)
        goto out;
    if (active == 0 || blink == 0) {
        col = find_colony(g_units[u].x, g_units[u].y);
        if (col >= 0)
            goto out;
    }
    flags = (active != 0) ? 0x80 : 0xc0;
    px = (g_units[u].x - g_view_x0) * g_tile_w;
    py = (g_units[u].y - g_view_y0) * g_tile_h;
    x = (g_tile_w >> 1) + px;
    y = py;
    if (NATION(g_units[u]) != ((unsigned char)g_game.view_nation))
        flags |= 0x20;
    PieceDraw_noport(u, flags, x, y, g_tile_w, g_icon_h);
out:
    ;
}
