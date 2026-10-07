/* GenerateActiveUnit: candidate for 1040:0e2e (92 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: GenerateActiveUnit
 *
 * Redraws the unit at SEG20:0x7794 through GenerateUnitStack: with `g_unit_blinking` set as
 * (1, 0) and, when `flag` is set, (1, 1) as well; otherwise as (0, 0).  The painter is
 * -called and defined in-file as a stub.
 *
 * `g_unit_blinking` (DGROUP:0x1ccc) is the flag this tests. `GenerateUnitStack(u, 1, 0)` returns
 * without drawing and `(1, 1)` draws the blink frame, so a set flag means the selected unit is
 * shown ONLY on the blink phase; a clear one takes the `(0, 0)` path and draws it solid.
 * `GameBrains_EndUnitTurn` and `orders_activate` both clear it across their `DrawMapRegion` call
 * and set it again afterwards, which is how the unit being left behind is repainted solid
 * before the new one starts blinking.
 *
 * RENAMED 2026-09-26 from refresh_arrived_unit_1040_0e2e: it repaints the selected unit (SEG20:0x7794) through GenerateUnitStack, solid or on the blink phase; the address suffix only told it apart from 1008:006f, which is now orders_hold and was never the same function.
 *
 * RENAMED 2026-09-26 from repaint_selected_unit: the Mac build's GenerateActiveUnit(short), bound by call-sequence alignment (1.0 aligned gaps in GameBrains_CursorUpdate leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.09x this one. It repaints the selected unit solid or on its blink phase; the descriptive name given earlier the same day is replaced by the developers' own.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern int g_stub_a;

extern void GenerateUnitStack(int u, int a, int b);

void GenerateActiveUnit(int flag)
{
    if (g_unit_blinking != 0) {
        GenerateUnitStack(g_game.selected_unit, 1, 0);
        if (flag != 0)
            GenerateUnitStack(g_game.selected_unit, 1, 1);
    } else {
        GenerateUnitStack(g_game.selected_unit, 0, 0);
    }
}
