/* strategy_routine: candidate for 1048:b93a (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Reads the viewing nation at SEG20:0x7798 into a local and does nothing with
 * it: whatever this reported was removed, and the read survives because the
 * local is assigned.
 *
 * RENAMED 2026-09-26 from show_strategy: the Mac build's strategy_routine(void), bound by call-sequence alignment: in MainWin_MainMenuProc the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (0), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

void strategy_routine(void)
{
    int view_nation = g_game.view_nation;
}
