/* start_timer: candidate for 1068:9014 (166 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: start_timer
 *
 * Starts a timer through the shared MrTimer at DGROUP:0x2ae8, creating it on first use, and
 * returns the slot MrTimer_Set hands back.
 */

#include "colonization.h"

#include "classes.h"

extern MrTimer * g_timer;   /* DGROUP:0x2ae8, declared */
extern int MrTimer_Set(MrTimer *timer, void *fn, int ms, int count);

extern "C" int start_timer(void *fn, int ms, int count)
{
    if (g_timer == 0)
        g_timer = new MrTimer;
    return MrTimer_Set(g_timer, fn, ms, count);
}
