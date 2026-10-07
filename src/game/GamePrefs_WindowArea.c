/* GamePrefs_WindowArea: candidate for 1040:7420 (80 bytes).  Verdict: RELOCATABLE.
 * symbol: GamePrefs_WindowArea
 * name-source: sibling-build
 *
 * Copies the rectangle at +2 of the entry GamePrefs_GetWindow finds for (root, which)
 * into *r; nothing when there is none.  The finder is -called and
 * defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from get_entry_rect: the Mac build's GamePrefs::WindowArea(short, Rect*): this plus a window id and a rectangle, filled with the saved placement. It sits in the GamePrefs module (1040:70fe..7470) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern char g_stub_s[];

extern char *GamePrefs_GetWindow(void *root, int which);

void GamePrefs_WindowArea(GamePrefs *root, int which, RECT *r)
{
    char *p;

    p = (char *)GamePrefs_GetWindow(root, which);
    if (p == 0)
        return;
    *r = *(RECT *)(p + 2);
}
