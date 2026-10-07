/* SetDifficulty_OkButtonClick: candidate for 1030:08c4 (87 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetDifficulty_OkButtonClick
 * name-source: sibling-build
 *
 * A dialog button handler of module 1030: the current window's owner gets its done flag
 * (+0x15a) set, and its cancelled flag (+0x15c) too when the button code is 0x65 -- Cancel's
 * id: SetDifficulty_Init builds "~Ok" as 0x64 and "~Cancel" as 0x65, and returns 0 when the
 * flag is set.
 *
 * RENAMED 2026-09-26 from dialog_click_15a: the OK/Cancel action SetDifficulty_Init installs on both buttons: sets the
 * owner's done flag, and its cancelled flag too for the Cancel button (code 0x65).
 *
 * RENAMED 2026-09-27 from difficulty_dialog_button: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). Found while checking lead 1. The header of SetDifficulty_Init.c says 'The Mac SetDifficulty has no LeftMouseClick or OkButtonClick of its own'. That is false. Both exist as long-form MacsBug names (0x80, length byte, name), which tools/mac_symbols.py does not read: CODE 17 RTS at 0x45b2 is followed by 'OkButtonClick__13SetDifficultyFs', closing 0x45b4..0x45e2. Layout: Mac puts it after the SetNations dtor and first in the SetDifficulty block. Windows puts 08c4 directly after choose_nation and first in the SetDifficulty block. Arity: (short) against (int code). Body and size: 0x57 bytes with the sole callee GetEventWindow, the same size and callee as its twin SetNations_OkButtonClick (1030:0000, 0x57). It sets the owner's done flag, and cancelled too for id 0x65. It has no direct callers on either side; it is installed as a handler. No collision.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

void SetDifficulty_OkButtonClick(int code)
{
    SetDifficulty *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (SetDifficulty *)w;
    o->done = 1;
    if (code == 0x65)
        o->cancelled = 1;
}
