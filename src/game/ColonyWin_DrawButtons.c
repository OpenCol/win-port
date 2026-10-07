/* ColonyWin_DrawButtons: candidate for 1010:b03a (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * The BODY says nothing about the parameters -- it reads none of them. The argument list
 * here is the one `colony_change_selected.c` declares for this address, which is where it can be
 * seen.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1010_b03a: the Mac build's ColonyWin::DrawButtons(short, short): this plus two shorts, called on the colony window by ColonyWin_DrawColony (with 0, 0) and by colony_change_mode and the food tutorial (with 1, 0) among the other panel painters. The Windows body is empty -- the colony buttons are native controls here -- and the name comes from those calls, not from the empty-function family. The width matches.
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;
void ColonyWin_DrawButtons(ColonyWin *unused1, int unused2, int unused3)
{
}
