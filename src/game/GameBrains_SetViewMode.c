/* GameBrains_SetViewMode: candidate for 1008:2d0d (109 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Redraws the square the selected unit is on. SEG20:0x7792 is latched to 1 first and
 * SEG20:0x7794 is the selected unit, so a negative value means nothing is selected and
 * only the trailing call runs.
 *
 * IT TAKES AN ARGUMENT IT DOES NOT USE, and the byte-match cannot see that: an unused 
 * parameter changes nothing in the body, so `(void)` and `(void *)` compile to the same
 * 109 bytes. The call sites settle it. All FOUR callers push four bytes and clean up with
 * `ADD SP,4`, and all four push the same pointer, `SEG20:0x40d6` -- a segment immediate
 * followed by an offset, which is the -pointer push shape (rule 25). One constant
 * argument at every site, ignored at the callee.
 *
 * This is finding 82 used as a CHECK rather than as a shortcut: the call sites state the
 * argument list, and here they state one the function itself gives no evidence for.
 *
 * RENAMED 2026-09-26 from view_pieces: the Mac build's GameBrains::SetViewMode(void): its one parameter is the unread `this`; it latches the view-mode word (SEG20:0x7792) to 1 and redraws the selected unit's square. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void DrawMapRegion(int x, int y, int a, int b, int flag);

extern void FreezeMenus(void);

void GameBrains_SetViewMode(GameBrains *brains)
{
    g_game.view_mode = 1;
    if (g_game.selected_unit >= 0)
        DrawMapRegion(g_units[g_game.selected_unit].x, g_units[g_game.selected_unit].y, 1, 1, 1);
    FreezeMenus();
}
