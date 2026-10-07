/* PORT: this file's own declarations of GenerateUnitStack removed: the header declares it from the definition (W1 step 3). */
/* GenerateUnit: candidate for 1040:0e8a (265 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws one unit, and decides whether it gets the ARRIVED-UNIT treatment instead of a plain
 * repaint. `GenerateUnitsRegion` calls it once per unit in the band it is refreshing.
 *
 * Seven conditions have to hold at once, and all seven are about the arrived unit rather
 * than the one being drawn: the game must be in state 0, two flags at DGROUP:0x88 and 0x8a
 * must be clear, the arrived unit's nation must be one of the four European powers, that
 * nation's controller byte at SEG20:0x7841 must be clear, `unit_is_active` must
 * agree, and the unit being drawn must share BOTH coordinates with it. Anything else and it
 * is painted flat through `GenerateUnitStack(u, 0, 0)`.
 *
 * So the blink follows the SQUARE, not the unit: every unit stacked on the arrived unit's
 * tile is drawn through the same highlighted path.
 *
 * THE NATION MASK IS EIGHT BITS, and that is rule 58. The shipped `AND AL,0xF` says the
 * source writes `(unsigned char)(flags & 0xf)`; without the cast C promotes and Borland masks
 * in AX. A four-bit bitfield narrows the COMPARE but not the mask, so it is not that either.
 * The `MOV AH,0` that widens the value appears only at the second use, right before the
 * `IMUL AX,AX,0x34` that indexes the nation record.
 *
 * `u` is a `register` PARAMETER: `MOV SI,[BP+6]` is the first thing after the prologue, with
 * nothing in between (rule 40).
 *
 * The byte read here is `g_nation_text[n].control`: +1 of the 0x34-byte per-nation record at
 * SEG20:0x7840, so the test is "this unit belongs to the human player". It used to be spelled
 * `g_recs[n].f0` -- the same byte seen from a base one higher -- which is why it looked
 * like a nameless first byte.
 *
 * RENAMED 2026-09-26 from draw_unit: the Mac build's GenerateUnit(short), bound by call-sequence alignment (1.0 aligned gaps in GenerateUnitsRegion leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.94x this one.
 */

#include "colonization.h"

#include "control.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int unit_is_active(int u);

/* same object: 4-byte `push cs; call ` */
extern void GenerateActiveUnit(int flag);

void GenerateUnit(register int u)
{
    if (g_game.view_mode == 0 && g_autoplay == 0 && g_debug_mode == 0
        && NATION(g_units[g_game.selected_unit]) < 4
        && g_nation_text[NATION(g_units[g_game.selected_unit])].control == CONTROL_HUMAN
        && unit_is_active(g_game.selected_unit) != 0
        && g_units[u].x == g_units[g_game.selected_unit].x
        && g_units[u].y == g_units[g_game.selected_unit].y)
        GenerateActiveUnit(1);
    else
        GenerateUnitStack(u, 0, 0);
}
