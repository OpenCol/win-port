/* AddMultiIcon: candidate for 1038:44bd (90 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends one entry to the colony window's production row: a goods kind, a count and the `part`
 * of that count to overlay, into the three nine-word tables at SEG26:0x0, 0x12 and 0x24, with the
 * entry count at DGROUP:0x4c3a. An entry whose count and part are both zero is dropped.
 *
 * IT WAS `push_undo_1038_44bd` AND THERE IS NO UNDO HERE. `DrawMultiIconRow` had already
 * written the correction into its own header -- "that second name is a misreading: it appends a
 * row entry and undoes nothing" -- but the name and the three `g_undo_*` arrays stayed, so the
 * callers still read as if they were pushing undo state. They are building a bar:
 * `ColonyWin_DrawColonists` adds the crosses with `(0x39, g_prod[PROD_CROSSES], 0)`, and the food
 * bar is two entries, what is eaten and the surplus.
 *
 * The struct is `DrawMultiIconRow`'s, which reads back what this writes.
 *
 * RENAMED 2026-09-26 from goods_row_add: the Mac build's AddMultiIcon(short, short, short), bound by layout (between ComputeIconRow and DrawIconRow on both sides); the arity agrees and the Mac body is 1.13x this one.
 */

#include "colonization.h"

#include "records.h"

void AddMultiIcon(int kind, int count, int part)
{
    if (count == 0 && part == 0)
        return;
    g_row.kind[g_row_count] = kind;
    g_row.count[g_row_count] = count;
    g_row.part[g_row_count] = part;
    g_row_count++;
}
