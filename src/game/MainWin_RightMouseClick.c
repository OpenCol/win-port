/* MainWin_RightMouseClick: candidate for 1048:a6db (60 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MainWin_RightMouseClick
 *
 * Hands the click to track_popup_submenu with the current window and its member at +0x8c.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class PediaWin { public: char body[0x3b0c - 0x32b2]; PediaWin(); ~PediaWin(); };
class MainWin : public EventWin { public: MenuClass menu; MapWin map; MiniWin mini; StatWin stat; ReportWin report; ColonyWin colony; EuropeWin europe; PediaWin pedia; MainWin(); ~MainWin(); };
extern void *GetEventWindow(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void MainWin_RightMouseClick(int x, int y)
{
    MainWin *w = (MainWin *)(EventWin *)GetEventWindow();

    track_popup_submenu(w, &w->menu, x, y);
}
