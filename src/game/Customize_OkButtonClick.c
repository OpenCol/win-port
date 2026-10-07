/* Customize_OkButtonClick: candidate for 1030:11ea (87 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize_OkButtonClick
 * name-source: sibling-build
 *
 * A dialog button handler of module 1030: the current window's owner gets its done flag
 * (+0x19e) set, and its cancelled flag (+0x1a0) too when the button code is 0x65 -- Cancel's
 * id: Customize_Init builds "~Ok" as 0x64 and "~Cancel" as 0x65, and returns 0 when the flag
 * is set.
 *
 * RENAMED 2026-09-26 from dialog_click_19e: the OK/Cancel action Customize_Init installs on both buttons: sets the
 * owner's done flag, and its cancelled flag too for the Cancel button (code 0x65).
 *
 * RENAMED 2026-09-26 from world_dialog_button: the Mac build's Customize::OkButtonClick(short): Customize_Init installs it as the action of both buttons, and it takes the button code.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

void Customize_OkButtonClick(int code)
{
    Customize *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (Customize *)w;
    o->done = 1;
    if (code == 0x65)
        o->cancelled = 1;
}
