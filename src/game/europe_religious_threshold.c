/* europe_religious_threshold: candidate for 1048:7757 (320 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The immigration arithmetic, both halves of it. The return value is the CROSSES REQUIRED for
 * the next immigrant and goes to +0x30 of the nation record; the word the caller passes by
 * address is the crosses PRODUCED this turn and is added to +0x2e, which the caller then
 * clamps at zero and compares against +0x30. That comparison is what makes the two numbers
 * legible: accumulator against threshold.
 *
 * The threshold is `2 * (colonists in this nation's colonies + units it owns) + 8`, capped at
 * 4000 -- and the doubling is skipped once the total is already at the cap, which is the only
 * reason the `< 0xfa0` test exists. Then a difficulty scaling, `(8 - g_game.difficulty) * t >> 3`,
 * for any nation other than a European power with a clear byte in the 0x34-stride table, and a
 * further two thirds for nation 0.
 *
 * The production starts at 2 and is knocked DOWN by units already waiting on the Europe
 * square, which this build puts at `(nation - 20, nation - 20)` -- `ADD DL,0xEC` is that
 * subtraction done in a byte. Each such unit that `job_of_unit` accepts, for a nation
 * whose record has bit 0x40 set, either drops the rate straight to -2 or takes two more off an
 * already negative one.
 *
 * `total = colonists + total` and NOT `total += colonists`: the shipped adds the byte's
 * register into the running total (`ADD AX,DI / MOV DI,AX`), which is the left operand in AX.
 * Written the other way round the two swap and one byte moves.
 *
 * The colony owner and the unit's nation are both compared in eight bits, so both sides are
 * cast: rule 58 for the mask, and `(unsigned char)nation` for the comparison.
 *
 * RENAMED 2026-09-27 from crosses_required: CONST PROPOSAL. Mac europe_religious_threshold(short, short*), CODE 25 0x3578, 318 bytes, called only by europe_new_turn, with callee job_of_unit. 7757 (int nation, int *rate), 320 bytes, is called only by europe_new_turn and calls job_of_unit and a min stub. Layout: Mac europe_purchase, europe_religious_threshold, europe_new_wars, europe_new_taxes, europe_new_turn; Windows europe_purchase (7296), [empty stubs], 7757, europe_new_wars, europe_new_taxes, europe_new_turn. Constants: overlap 0.82, no runner-up.
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];           /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */

extern int job_of_unit(int u);

/* same object: 4-byte `push cs; call ` */

int europe_religious_threshold(int nation, int *rate)
{
    register int i;
    register int total;

    *rate = 2;
    total = 0;
    for (i = 0; g_game.colony_count > i; i++) {
        if (g_colonies[i].nation == (unsigned char)nation)
            total = g_colonies[i].pop + total;
    }
    for (i = 0; g_game.unit_count > i; i++) {
        if (NATION(g_units[i]) == (unsigned char)nation) {
            total = total + 1;
            if (g_units[i].x == (unsigned char)(nation + 0xec)
                && job_of_unit(i) >= 0
                && (g_nations[nation].flags & 0x40)) {
                if (*rate > 0)
                    *rate = -2;
                else
                    *rate -= 2;
            }
        }
    }
    if (total < 0xfa0)
        total = total * 2;
    total += 8;
    total = min_int_1048_db44(total, 0xfa0);
    if (g_cur_nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_nation].control != CONTROL_HUMAN)
        total = (8 - g_game.difficulty) * total >> 3;
    if (g_cur_nation == NATION_ENGLAND)
        total = total * 2 / 3;
    return total;
}
