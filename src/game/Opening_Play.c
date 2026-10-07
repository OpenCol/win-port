/* Opening_Play: candidate for 1010:04b6 (74 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_Play
 * name-source: sibling-build
 *
 * Runs the opening: a 150 ms timer on Opening_TimerProc, the message pump
 * (MSDoTask_1010_2b2a) until the Opening's sequence_done byte (+0x1e76) is set, then the
 * timer is reset and MSDoAllTasks_1010_2a54 cleans up.  The two pump helpers are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from opening_run: the Mac build's Opening::Play(void): this only, starting the 150 ms timer and pumping until the Opening's sequence_done byte is set. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
extern int g_stub_a;

extern void Opening_TimerProc(void);

void Opening_Play(Opening *w)
{
    int timer;

    timer = start_timer((void *)Opening_TimerProc, 150, -1);
    while (w->sequence_done == 0)
        MSDoTask_1010_2b2a();
    ResetTimer(timer);
    MSDoAllTasks_1010_2a54();
}
