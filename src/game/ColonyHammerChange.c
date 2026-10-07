/* ColonyHammerChange: candidate for 1010:734b (30 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `ColonyWin_colony_hammer_change` with the colony window record
 * at SEG20:0x25be, named for the routine it reaches.
 *
 * The callee is -called and is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from rec202_7c18: the button action ColonyWin_Init installs on its button at
 * +0x972: ColonyWin_colony_hammer_change on the colony window with the id -- ColonyHammerBuy's neighbour.
 *
 * RENAMED 2026-09-27 from colony_choose_build_clicked: BEYOND THE PROPOSALS. Mac ColonyHammerChange(short), CODE 30 0x05e8, 48 bytes: LINK; move.w 8(a6),-(sp); pea g_colony_window (A5 $65e4); jsr colony_hammer_change; UNLK; RTS. 734b (30 bytes) is colony_choose_build(&g_colony_window, id), which is the same body. Arity: (short) against (int id). No direct callers on either side; both are installed as a button action. Layout: Mac colony_hammer_change, ColonyHammerChange, ColonyHammerBuy, colony_hammer_buy; Windows colony_choose_build, 734b, 7369, ColonyWin_colony_hammer_buy. The Mac has its own name for this thunk, so it takes that name rather than a coined one.
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` */
extern void ColonyWin_colony_hammer_change(void *w, int a);

void ColonyHammerChange(int id)
{
    ColonyWin_colony_hammer_change(&g_colony_window, id);
}
