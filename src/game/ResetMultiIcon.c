/* ResetMultiIcon: candidate for 1038:44a9 (20 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Starts the colony window's production row: clears the entry count at DGROUP:0x4c3a so the
 * next `AddMultiIcon` writes slot 0. `ColonyWin_DrawMakeList` names the three-step shape --
 * this starts the row, `AddMultiIcon` appends an entry, `DrawMultiIconRow` draws it -- and the
 * count is `g_row_count`, which `AddMultiIcon` declares at the same address.
 *
 * The nine-word tables at SEG26 are NOT cleared: only the count is, so stale entries past the
 * new count are simply never read.
 *
 * RENAMED 2026-09-27 from goods_row_reset: the Mac build's ResetMultiIcon, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Clears g_row_count so the next AddMultiIcon writes slot 0; the reset of the AddMultiIcon/DrawMultiIconRow trio; called by ColonyWin_DrawColonists and DrawProduction, as the Mac is by DrawColonists and DrawMakeList; 0 args.
 */

#include "colonization.h"

void ResetMultiIcon(void)
{
    g_row_count = 0;
}
