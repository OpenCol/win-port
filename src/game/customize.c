/* customize: candidate for 1030:1c3e (111 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: customize
 * name-source: sibling-build
 *
 * Builds a Customize window on the stack, runs it, and reports whether the player backed
 * out. `begin_game` calls this and only this when the opening-menu choice is 3, and
 * treats a non-zero answer as "abandon the new game" -- so the dialog body at 1030:1397
 * returns zero on cancel and this wrapper turns that round.
 *
 * The object is 0x1a4 bytes of local frame and has a destructor, which is where the
 * exception block comes from: `__InitExceptBlock`, the state word set to 6, and the
 * handler restored on the way out. `PUSH AX` around the destructor call is the return
 * value being carried across it.
 *
 * The body's answer is a BYTE -- `MOV AH,0` after the call, then a 16-bit store -- so the
 * dialog returns `unsigned char` and the comparison is done on the widened value.
 *
 * RENAMED 2026-09-27 from run_customize_dialog: Mac customize(void). Caller: begin_game on both (begin_game calls it for difficulty 3). Layout: after Customize::DoMessage on both (Mac CODE 17 TrackMouse, DoMessage, customize; Windows Customize_TrackMouse, Customize_DoMessage, 1c3e). Callee: Customize::Init on both. Windows builds the Customize object with Customize_ctor/dtor, whose copies sit at the segment end (1030:e9d2/ea51). The Mac inlines the constructor's members (GRPort ctor, Screen ctor, EventTable::ClearEventTable), which accounts for the size (872 Mac against 111). Arity: void.
 * Customize::Customize, Customize_Init and Customize::~Customize are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" extern "C" int Customize_Init(Customize *c);

extern "C" int customize(void)
{
    Customize c;
    int cancelled = 0;
    int answer;

    answer = (unsigned char)Customize_Init(&c);
    if (answer == 0)
        cancelled = 1;
    return cancelled;
}
