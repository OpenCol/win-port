/* site_routine: candidate for 1048:b92c (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * A Cheat-menu entry that does nothing in the shipped build. The body is empty and the
 * function survives only because its address is in the command table.
 *
 * RENAMED 2026-09-26 from show_colony_sites: the Mac build's site_routine(void), bound by call-sequence alignment: in MainWin_MainMenuProc the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (0), and the body does what the Mac name says.
 */

#include "colonization.h"

void site_routine(void)
{
}
