/* set_wing_orientation: candidate for 1070:a637 (20 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores its argument in the WinG orientation word at DGROUP:0x39b4 and nothing else.
 * `MSInitGraf` is what names that global: -1 selects a bottom-up bitmap, 1 a top-down one, and
 * anything else takes WinG's own recommendation.
 *
 * Its two callers bracket the closing sequence -- `ClosingWin_ctor` sets 1 and
 * `DoTheClosing` puts 0 back -- so the end-of-game animation is drawn top-down and the
 * rest of the game is left as WinG asked for.
 */

#include "colonization.h"

void set_wing_orientation(int n)
{
    g_wing_orientation = n;
}
