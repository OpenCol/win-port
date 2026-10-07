/* GamePrefs_WindowState: candidate for 1040:7470 (73 bytes).  Verdict: RELOCATABLE.
 * symbol: GamePrefs_WindowState
 * name-source: sibling-build
 *
 * The word at +0xa of the entry GamePrefs_GetWindow finds for (root, which), or -1.
 * The finder is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from find_entry_value: the Mac build's GamePrefs::WindowState(short): this plus a window id, the saved state code (MainWin_Init tests it for 2, maximised). It sits in the GamePrefs module (1040:70fe..7470) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern char g_stub_s[];

extern PlacementRec *GamePrefs_GetWindow(void *root, int which);

int GamePrefs_WindowState(GamePrefs *root, int which)
{
    PlacementRec *e;

    e = GamePrefs_GetWindow(root, which);
    if (e == 0)
        return -1;
    return e->state;
}
