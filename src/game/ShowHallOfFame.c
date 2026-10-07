/* ShowHallOfFame: candidate for 1050:6f2f (24 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Calls `HallOfFame(0, 0)` and nothing else -- a null entry and a zero flag, which is the
 * "just show it" call. Its one caller is the opening menu, `begin_game` (choice 5).
 *
 * RENAMED 2026-09-27 from show_hall_of_fame: Mac ShowHallOfFame(void), 40 bytes, against 24. Caller: begin_game on both. Callee: HallOfFame only. Layout: before HallOfFame on both (Windows has hall_of_fame_with_entry between them, Windows-only). Arity: void.
 */

#include "colonization.h"

#include "records.h"
extern void HallOfFame(FameEntry *entry, char show);

void ShowHallOfFame(void)
{
    HallOfFame(0, 0);
}
