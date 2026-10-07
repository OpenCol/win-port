/* hall_of_fame_with_entry: candidate for 1050:6f4a (30 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `HallOfFame(entry, 0)`: shows the Hall of Fame with a caller-supplied entry. Its neighbour
 * `ShowHallOfFame` (immediately before) calls `HallOfFame(0, 0)` -- no entry -- so this is
 * the same screen reached with one, the form a new score would take.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
 */

#include "colonization.h"

#include "records.h"
extern void HallOfFame(FameEntry *entry, char show);

void hall_of_fame_with_entry(FameEntry *entry)
{
    HallOfFame(entry, 0);
}
