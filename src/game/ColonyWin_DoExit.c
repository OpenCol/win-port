/* ColonyWin_DoExit: candidate for 1010:ae40 (35 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Returns 1 through a LOCAL, not directly: `SUB SP,2 / MOV WORD [BP-4],1 /
 * MOV AX,[BP-4]`. A bare `return 1;` materialises the constant in AX and costs no
 * frame at all, so the local is in the source and the function once did more with it.
 * One of TWO copies.
 *
 * 35 bytes, not the census 28: the written-out `return` adds a dead second epilogue
 * (rule 44).
 *
 * RENAMED 2026-09-26 from return_one_1010_ae40: the Mac build's ColonyWin::DoExit(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between ColonyWin_TimerProc and ColonyWin_OnOKClick, which are already bound; the arity agrees and the Mac body is 1.20x this one.
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;
int ColonyWin_DoExit(ColonyWin *unused1)
{
    int r = 1;

    return r;
}
