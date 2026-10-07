/* PORT: this file's own declarations of sprite_width_1048_e0ee removed: the header declares it from the definition (W1 step 3). */
/* EuropeWin_DrawTransitUnit: candidate for 1048:993a (706 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawTransitUnit
 * name-source: sibling-build
 *
 * Draws one unit of a Europe ship row and advances the row's counter, which it is handed BY ADDRESS
 * -- the reason `EuropeWin_DrawInbound` can walk two chains into one row. The cell comes from
 * `get_transit_loc`, the layout `EuropeWin_EuropeMouseShipsProper` hit-tests against, so what
 * this draws is what that clicks. `EuropeWin_DrawLandUnit` is the docks' version of the same
 * function on `get_unit_loc`.
 *
 * Tier 0 draws the unit at full size and tier 1 at half (`SetGlobalSpriteScale(1, 2)`), both offset
 * half the portrait's width plus five; a SHIP (UNIT_SHIP_FIRST..UNIT_SHIP_LAST) in tier 0 with cargo
 * aboard, when `extra` is below 2, also gets its first cargo's goods sprite at three-quarter size.
 * Tier 2 draws the type's small sprite (UnitType+2) at 100 / (tier * 2) per cent -- a quarter.
 * Anything in tier 3 or beyond is not drawn, but still counted. A non-negative `h` outlines the
 * cell.
 *
 * THE TIER-0 ARM SUBTRACTS `tier == 1 ? 4 : 0`, which inside `if (tier == 0)` is always 0. The
 * shipped code evaluates the ternary anyway: the x expression was copied between the two arms.
 *
 * The unit is a register COPY of the parameter, which is how it keeps SI while the portrait's
 * half-width takes DI (rule 53). `tier * 2` doubles in the accumulator (rule 63).
 *
 * RENAMED 2026-09-27 from europe_draw_ship: BEYOND THE PROPOSALS. Mac EuropeWin::DrawTransitUnit(short, short, short, short, short*, short), CODE 25 0x5a26, 754 bytes. 993a (EuropeWin *w, int u, int base_x, int count3, int extra, int *n, int colour) has the same shape: four words, a pointer, a word. Callers: Mac (bsr) DrawEuropeShips, DrawInbound, DrawOutBound; Windows EuropeWin_DrawEuropeShips, EuropeWin_DrawInbound x2, EuropeWin_DrawOutBound x2. Callees: set_europe, get_transit_loc (classify_index_into_tier), UnitIcon, PieceDraw x2, SetGlobalSpriteScale x6, get_cargo, Sprite::Draw x2, TextColor, DrawRect on both. Layout: directly after DrawTransit, before DrawLandUnit, on both. Size 754 against 706. EuropeWin_DrawTransit is a prefix of the new name, so rename by exact word. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









#include "records.h"

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"
extern int UnitIcon(int u);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y, int span, int mode);
extern void SetGlobalSpriteScale(int num, int den);
extern int get_cargo(int u, int slot);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_europe(int n);
extern "C" void get_transit_loc(int idx, int base_x, int tier3_count, int extra, int *tier, int *x, int *sprite, int *w, int *size);

void EuropeWin_DrawTransitUnit(EuropeWin *w, int u, int base_x, int count3, int extra,
                                  int *n, int colour)
{
    int tier;
    int cx;
    int cy;
    int cw;
    int ch;
    RECT r;
    RECT s1;
    RECT s2;
    register int unit = u;
    register int half;

    set_europe(w->nation);
    get_transit_loc(*n, base_x, count3, extra, &tier, &cx, &cy, &cw, &ch);
    half = sprite_width_1048_e0ee(&g_icon_sprites[UnitIcon(unit)]) / 2 + 5;
    if (tier < 2) {
        if (tier == 0) {
            PieceDraw((GRPort *)w, unit, 0, w->rect.left + half + cx - (tier == 1 ? 4 : 0), w->rect.top + cy, 0x20, 0x64);
        } else {
            SetGlobalSpriteScale(1, 2);
            PieceDraw((GRPort *)w, unit, 0, w->rect.left + half + cx - (tier == 1 ? 4 : 0), w->rect.top + cy, 0x20, 0x64);
            SetGlobalSpriteScale(1, 1);
        }
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST
            && tier == 0 && extra < 2 && g_units[unit].cargo_count != 0) {
            SetGlobalSpriteScale(3, 4);
            Sprite_Draw(&s1, &g_icon_sprites[ICON_GOODS + get_cargo(unit, 0)], (GRPort *)w,
                        w->rect.left + half + cx - 10, w->rect.top + cy - 5);
            SetGlobalSpriteScale(1, 1);
        }
    } else if (tier < 3) {
        SetGlobalSpriteScale(100 / (tier * 2), 100);
        Sprite_Draw(&s2, &g_icon_sprites[g_unit_types[g_units[unit].type].icon], (GRPort *)w,
                    w->rect.left + half + cx + (cw >> 1) - 1, w->rect.top + cy + ch - 1);
        SetGlobalSpriteScale(1, 1);
    }
    if (colour >= 0 && tier < 3) {
        SetRect(&r, cx, cy - 1, cx + cw, cy + ch);
        OffsetRect(&r, w->rect.left, w->rect.top);
        TextColor(0);
        GRPort_DrawRect((GRPort *)w, &r);
    }
    (*n)++;
}
