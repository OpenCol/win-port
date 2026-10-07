/* GamePrefs_GetWindow: candidate for 1040:7360 (81 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Finds the 12-byte entry with the given key in a table (count at +8,
 * entries from +0xc); null when absent.
 *
 * RENAMED 2026-09-26 from find_entry: the Mac build's GamePrefs::GetWindow(short): this plus a window id, the placement entry for that window; every caller passes the preferences root (g_prefs_root). It sits in the GamePrefs module (1040:70fe..7470) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

PlacementRec *GamePrefs_GetWindow(GamePrefs *prefs, int key)
{
    int i;

    for (i = 0; prefs->count > i; i++) {
        if (prefs->windows[i].id == key)
            return &prefs->windows[i];
    }
    return 0;
}
