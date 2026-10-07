/* SetNations_OkButtonClick: candidate for 1030:0000 (87 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetNations_OkButtonClick
 * name-source: sibling-build
 *
 * A dialog button handler of module 1030: the current window's owner gets its done flag
 * (+0x152) set, and its cancelled flag (+0x154) too when the button code is 0x65 -- Cancel's
 * id: SetNations_Init builds "~Ok" as 0x64 and "~Cancel" as 0x65, and returns 0 when the
 * flag is set.
 *
 * RENAMED 2026-09-26 from dialog_click_152: the OK/Cancel action SetNations_Init installs on both buttons: sets the
 * owner's done flag, and its cancelled flag too for the Cancel button (code 0x65).
 *
 * RENAMED 2026-09-26 from nation_dialog_button: the Mac build's SetNations::OkButtonClick(short): SetNations_Init installs it as the action of both buttons, and it takes the button code.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

void SetNations_OkButtonClick(int code)
{
    SetNations *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (SetNations *)w;
    o->done = 1;
    if (code == 0x65)
        o->cancelled = 1;
}
