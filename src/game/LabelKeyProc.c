/* LabelKeyProc: candidate for 1048:3d21 (44 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Points the current-window reference (DGROUP:0x2ae0) at the Europe window record
 * (SEG20:0x2ff8) and forwards the key to the Europe key handler (EuropeWin_EuropeKeyParse,
 * a call TLINK converted).
 *
 * RENAMED 2026-09-26 from europe_keyclick: the Mac build's LabelKeyProc(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between InBox and europe_label_cargo, which are already bound; the arity agrees and the Mac body is 1.23x this one.
 */

#include "colonization.h"

extern void EuropeWin_EuropeKeyParse(int key);

void LabelKeyProc(int key)
{
    g_current_window = &g_europe_win_dlg;
    EuropeWin_EuropeKeyParse(key);
}
