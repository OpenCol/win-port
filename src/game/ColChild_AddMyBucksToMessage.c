/* ColChild_AddMyBucksToMessage: candidate for 1048:11ff (40 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_AddMyBucksToMessage
 * name-source: sibling-build
 *
 * Appends nation n's gold (check_gold) to the window's message through
 * ColChild_AddBucksToMessage (-called, defined in-file as a stub).
 *
 * RENAMED 2026-09-27 from add_nation_money_to_message: 6 bytes, calls AddBucksToMessage and check_gold, as on the Mac
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;
extern long check_gold(int nation);

void ColChild_AddMyBucksToMessage(ColChild *w, int n)
{
    ColChild_AddBucksToMessage(w, check_gold(n));
}
