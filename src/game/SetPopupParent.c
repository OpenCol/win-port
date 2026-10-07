/* SetPopupParent: candidate for 1050:0000 (22 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Stores a 32-bit value at DGROUP:0x24fc -- the first function of segment 1050.
 *
 * RENAMED 2026-09-26 from popup_owner_set: the Mac build's SetPopupParent(EventWin&), bound by call-sequence alignment (3.0 aligned gaps in ColonyWin_Show, EuropeWin_Show leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 2.09x this one.
 */

#include "colonization.h"

#include "classes.h"

void SetPopupParent(EventWin *parent)
{
    g_popup_owner = parent;
}
