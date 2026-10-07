/* EuropeWin_EuropeMouseParse: candidate for 1048:19a9 (169 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMouseParse
 * name-source: sibling-build
 * size-source: 169, not the census 57. The switch is dense, so Borland puts its eleven-entry
 * jump table in the CODE segment at 0x1a3c and Ghidra's reachable-code size stops before it.
 *
 * Hands a click in region `r` of the window to that region's handler, with the coordinates
 * already made window-relative by the caller -- which computes them as `x - o->left` and
 * `y - o->top`, gets `r` from `EuropeMouseArea` on the same pair, and takes the
 * close-dialog path instead when the click is outside.
 *
 * The axes were the other way round in this header until `EuropeWin_EuropeShowBig`
 * settled them: that function hands +0x76a and +0x76c to `OffsetRect`, which is
 * `` and takes its x displacement first. Parameter names do not move bytes, so
 * this file still grades unchanged. `set_europe` runs first, off
 * the nation word at +0x77a of the window.
 *
 * THE CASE ORDER IS 0, 10, 1, 2/3, 4 AND THAT IS THE SOURCE ORDER. Borland lays the arms
 * out in the order they are written and the table indexes them, so the table entries run
 * 0x19da, 0x19fe, 0x1a10, 0x1a10, 0x1a22, then five copies of the default, then 0x19ec --
 * and reading them back gives case 0 first, case 10 second. Cases 5 through 9 fall to the
 * default and do nothing.
 *
 * The `default:` is explicit and empty. Without it the out-of-range `JA` would land on the
 * epilogue; the shipped lands on a `JMP SHORT +0` first, which is the arm's own `break`
 * jumping to the instruction after it.
 *
 * RENAMED 2026-09-27 from dispatch_region_click: BEYOND THE PROPOSALS: the Europe mouse block, like ColonyWin's. Mac EuropeWin::EuropeMouseParse(short, short, short), CODE 33 0x000c, 194 bytes. Its bsr calls are EuropeMouseMarket, EuropeMouseShipsCargo, EuropeMouseShipsProper, EuropeMouseTransit, EuropeMouseUnits. 19a9's calls are europe_market_click, europe_cargo_click, europe_ships_click, europe_voyage_click and EuropeWin_EuropeMouseUnits, plus set_europe on both. Callers: Mac LeftMouseClick and RightMouseClick; Windows EuropeWin_LeftMouseClick and _RightMouseClick. Arity: this+3 against (w, r, x, y). Layout: directly before EuropeMessage on both. Size 194 against 169. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void set_europe(int n);
extern "C" void EuropeWin_EuropeMouseMarket(EuropeWin *w, int x, int y);   /* PORT: C linkage, as defined (W2) */
extern "C" void EuropeWin_EuropeMouseShipsCargo(EuropeWin *w, int x, int y);   /* PORT: C linkage, as defined (W2) */
extern void EuropeWin_EuropeMouseShipsProper(EuropeWin *w, int x, int y);
extern void EuropeWin_EuropeMouseTransit(EuropeWin *w, int x, int y);
extern void EuropeWin_EuropeMouseUnits(EuropeWin *w, int x, int y);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void EuropeWin_EuropeMouseParse(EuropeWin *w, int r, register int x, register int y)
{
    set_europe(w->nation);
    switch (r) {
    case 0:
        EuropeWin_EuropeMouseMarket(w, x, y);
        break;
    case 10:
        EuropeWin_EuropeMouseShipsCargo(w, x, y);
        break;
    case 1:
        EuropeWin_EuropeMouseShipsProper(w, x, y);
        break;
    case 2:
    case 3:
        EuropeWin_EuropeMouseTransit(w, x, y);
        break;
    case 4:
        EuropeWin_EuropeMouseUnits(w, x, y);
        break;
    default:
        break;
    }
}
