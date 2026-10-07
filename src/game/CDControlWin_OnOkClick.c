/* CDControlWin_OnOkClick: candidate for 1040:9d9f (71 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets the word at +0x9e0 of the current window's owner object (the window record sits
 * 0x26 into it).
 *
 * RENAMED 2026-09-26 from cur_win_set_9e0: the action CDControlWin_Init installs on its button at +0x9e2:
 * sets the owner's done flag (+0x9e0), which ends the panel's loop.
 *
 * RENAMED 2026-09-26 from cd_panel_done_clicked: the Mac build's CDControlWin::OnOkClick(short): CDControlWin_Init installs it as the Done button's action. The body reads no argument, so the Windows prototype has none.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

void CDControlWin_OnOkClick(void)
{
    CDControlWin *o;
    char *w = (char *)GetEventWindow();

    o = (CDControlWin *)(w != 0 ? w - 0x26 : w);
    o->done = 1;
}
