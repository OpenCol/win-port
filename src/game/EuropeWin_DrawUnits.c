/* EuropeWin_DrawUnits: candidate for 1048:9df1 (298 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawUnits
 * name-source: sibling-build
 *
 * Redraws the Europe window's DOCK: clear the 0xc1 x 0x93 panel at (0x1bf, 0x102), then walk
 * the unit chain standing on the Europe square -- `(g_cur_nation - 20, g_cur_nation - 20)`,
 * this build's off-board Europe -- and draw every unit that is NOT a ship. Types 0xd..0x12 are
 * the ships and are skipped here; they belong to the harbour panel.
 *
 * The running position and the running index are both passed to the per-unit drawer, the index
 * BY ADDRESS, so it is the drawer that advances the count and this loop never touches it.
 *
 * `h` is the highlight: -1 for an ordinary unit, 10 for the selected one, 15 when the window
 * is in state 4 or in state 2 with DGROUP:0x4cd8 set. Two different conditions promote the
 * same value, written as two separate `if`s and not an `||` -- the shipped tests both even
 * after the first has already set 15.
 *
 * The final blit is optional and its rectangle is in WINDOW coordinates: the panel's corner
 * plus the window's own origin at +0x76a/+0x76c, which is the same pair `MiniWin_UpdateTerrain`
 * adds and does not add to an extent.
 *
 * `g_4cd8` KEEPS ITS ADDRESS FOR A NAME, deliberately. Nothing in the 22,620 bytes of DGROUP
 * ever writes it -- `tools/check_dead_dgroup.py` finds two instructions touching 0x4cd8 in the
 * whole image and both are this `cmp word [0x4cd8],0x0`, in this file and its neighbour. It
 * ships zero, and its neighbours `g_cur_nation` at 0x4cc8 and `g_cheat_step` at 0x4cf6 are both
 * stored to absolutely, so no block load covers it either. The guard is therefore decided
 * before the game starts and this arm cannot run. A role name like `g_selection_flash` would
 * read as a flash the image never performs, so the address stays and this note carries the
 * meaning instead.
 *
 * RENAMED 2026-09-26 from europe_draw_dock_1048_9df1: the Mac build's EuropeWin::DrawUnits(short): this plus a flag, painting every unit on the Europe square that is not a ship -- the colonists on the dock.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
#include "classes.h"
#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

/* same object: 4-byte `push cs; call ` */
extern "C" void set_europe(int n);
extern "C" void EuropeWin_UpdateEurope(EuropeWin *o, int x, int y, int w, int h);
extern "C" void EuropeWin_DrawLandUnit(EuropeWin *o, int u, int x, int *n, int h);
extern "C" void copy_rect_to_screen(EuropeWin *win, RECT *r);

void EuropeWin_DrawUnits(EuropeWin *o, int blit)
{
    int x;
    int n;
    RECT r;
    register int u;
    register int h;

    set_europe(o->nation);
    EuropeWin_UpdateEurope(o, 0x1bf, 0x102, 0xc1, 0x93);
    x = 0x1d2;
    n = 0;
    u = top_unit_in_square(g_cur_nation - 20, g_cur_nation - 20);
    while (u >= 0) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
            h = -1;
            if (n == g_land_unit_index) {
                h = 10;
                if (g_drag_mode == 4)
                    h = 15;
                if (g_selection_kind == 2 && g_4cd8 != 0)
                    h = 15;
            }
            EuropeWin_DrawLandUnit(o, u, x, &n, h);
        }
        u = next_unit(u);
    }
    if (blit != 0) {
        SetRect(&r, o->rect.left + 0x1bf, o->rect.top + 0x102, o->rect.left + 0x280, o->rect.top + 0x195);
        copy_rect_to_screen(o, &r);
    }
}
