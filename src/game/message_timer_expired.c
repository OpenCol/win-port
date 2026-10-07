/* PORT: this file's own declarations of MSSetWinTitle removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 38 of the imported file. */
/* message_timer_expired: candidate for 1048:0000 (217 bytes).  Verdict: RELOCATABLE.
 * symbol: message_timer_expired
 *
 * The message-timer callback: whichever of the three windows' pending timer ids
 * (SEG20:0x3748 Europe, 0xdce map, 0x2d34 colony) matches is cleared and that window's message
 * line is refreshed -- the Europe window through EuropeWin_DrawTitle, the map window by writing
 * "Area Map" as its caption, the colony window through ColonyWin_DrawTitle -- and redrawn with
 * ColChild_PackQueue. The caption wrappers are defined here because the shipped calls to them are
 * the compiler's own `push cs / call ` (their own callee ColChild_DrawTitleBar is not in this
 * file); the refresh calls are calls TLINK converted.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

typedef struct { char pad[1]; } EuropeWin;

typedef struct { char pad[1]; } ColonyWin;

#include "records.h"

extern void ColChild_DrawTitleBar(ColChild *w);
extern void EuropeWin_DrawTitle(EuropeWin *w);
extern void ColChild_PackQueue(ColChild *w);
extern void ColonyWin_DrawTitle(ColonyWin *w);

extern void win_set_title(DialogWin *w, char *s);

extern void win_set_caption_1048_d48b(ColChild *w, char *s);

void message_timer_expired(int id)
{
    if (g_europe_timer != -1 && g_europe_timer == id) {
        g_europe_timer = -1;
        EuropeWin_DrawTitle((EuropeWin *)(&g_europe_win));
        ColChild_PackQueue((ColChild *)&g_europe_win);
    }
    if (g_map_timer != -1 && g_map_timer == id) {
        g_map_timer = -1;
        win_set_caption_1048_d48b((ColChild *)&g_map_window, "Area Map");
        ColChild_PackQueue((ColChild *)&g_map_window);
    }
    if (g_colony_timer != -1 && g_colony_timer == id) {
        g_colony_timer = -1;
        ColonyWin_DrawTitle((ColonyWin *)&g_colony_window);
        ColChild_PackQueue((ColChild *)&g_colony_window);
    }
}
