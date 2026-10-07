/* Opening_TimerProc: candidate for 1010:0500 (202 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_TimerProc
 * name-source: sibling-build
 *
 * One tick of the opening sequence (the Opening object at DGROUP:0x964): unless the state
 * (+0x550) is 4 the animation frame advances, the screen is composed (Opening_DoShip), and in
 * every state but 4 the sequence's parts run in turn -- Opening_DoCredits, 0cee, 0eb3, the active
 * animations, Opening_UnDoCredits -- after which a SKIP request of 1 at +0x56a -- what a click
 * in any state but 4 sets -- moves to state 3 and recomposes. That byte (`skip`) was called `done`
 * here, a name that fits a different one: +0x1e76 (`sequence_done`), the flag `Opening_Play` waits on. Every call is a call TLINK converted to its NOP-padded `push cs / call `.
 *
 * RENAMED 2026-09-26 from intro_tick: the Mac build's Opening::TimerProc(short, short): the callback Opening_Play hands start_timer. The body reads neither timer argument (it works on the global Opening), so the Windows prototype has none. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
extern Opening *g_opening;   /* DGROUP:0x964 */
extern void Opening_DoMonsters(Opening *o);
extern void Opening_DoShip(Opening *o);
extern void Opening_DoCredits(Opening *o);
extern void Opening_DoPan(Opening *o);
extern void Opening_UnDoShip(Opening *o);
extern void Opening_UnDoMonsters(Opening *o);
extern void Opening_UnDoCredits(Opening *o);

void Opening_TimerProc(void)
{
    if (g_opening != 0) {
        if (g_opening->state != 4)
            Opening_DoMonsters(g_opening);
        Opening_DoShip(g_opening);
        if (g_opening->state != 4) {
            Opening_DoCredits(g_opening);
            Opening_DoPan(g_opening);
            Opening_UnDoShip(g_opening);
            Opening_UnDoMonsters(g_opening);
            Opening_UnDoCredits(g_opening);
            if ((int)g_opening->skip == 1) {
                g_opening->state = 3;
                Opening_DoShip(g_opening);
            }
        }
    }
}
