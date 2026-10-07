/* IsBackdrop: candidate for 1008:4bd3 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 25. The last 8 are a second, unreachable epilogue -- `pop ds / pop bp /
 * dec bp / retf` twice over -- which the comparator carries without complaint.
 *
 * Returns the pointer at DGROUP:0xc6 and nothing else -- two loads into DX:AX.
 *
 * That global is `g_backdrop_win`, the desktop-sized EventWin `OpenBackdrop` builds and
 * `CloseBackdrop` deletes. Its six callers all use it the same way: `parent = ...`, and
 * then build their window over it when it is non-zero and over `g_main_win` when it is not.
 *
 * RENAMED 2026-09-26 from overlay_window: the Mac build's IsBackdrop(void), bound by call-sequence alignment (1.5 aligned gaps in SetNations_Init, SetDifficulty_Init, Customize_Init leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.28x this one.
 */

#include "colonization.h"

#include "classes.h"
extern EventWin *g_backdrop_win;   /* DGROUP:0xc6 */

EventWin *IsBackdrop(void)
{
    return g_backdrop_win;
}
