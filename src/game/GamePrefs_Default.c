/* GamePrefs_Default: candidate for 1040:72e0 (128 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Fills a display configuration from the desktop size: two derived values, a 32-bit 7,
 * two flags, and eight zeroed slots at +0x18c.
 *
 * RENAMED 2026-09-26 from display_config_init: the Mac build's GamePrefs::Default(void): this only, filling the preferences with defaults from the desktop size; GamePrefs_LoadPrefs calls it on its own object when there is nothing to load. It sits in the GamePrefs module (1040:70fe..7470) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void GamePrefs_Default(GamePrefs *prefs)
{
    char rect[8];
    int i;

    MSGetDesktopSize(rect);
    prefs->w = rect_width_1040_a903((RECT *)rect);
    prefs->h = rect_height_1040_aa80((RECT *)rect);
    prefs->f0 = 7;
    prefs->count = 0;
    prefs->use_default_layout = 1;
    for (i = 0; i < 8; i++)
        prefs->slots[i] = 0;
}
