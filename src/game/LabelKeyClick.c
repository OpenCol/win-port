/* LabelKeyClick: candidate for 1010:36fe (44 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Points the current-window reference (DGROUP:0x2ae0) at the colony window record
 * (SEG20:0x25e4) and forwards the key to ColonyWin_KeyClick.
 *
 * RENAMED 2026-09-26 from colony_keyclick: the Mac build's LabelKeyClick(short), bound by layout (between label_remove_label and label_building on both sides); the arity agrees and the Mac body is 1.23x this one.
 */

#include "colonization.h"

#include "classes.h"

void LabelKeyClick(int key)
{
    g_current_window = &g_colony_win_rec;
    ColonyWin_KeyClick(key);
}
