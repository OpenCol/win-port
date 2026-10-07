/* build_check: candidate for 1040:843c (173 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: build_check
 *
 * Every 600 ticks of the clock (MSGetTicks_1040_aaa0, last time at DGROUP:0x1d0c)
 * plays the next of ten items (show_ship by 0x1d08) or, after the
 * tenth, raises the done flag at 0x1d06; then, when 0x1d04 is 1 with
 * `flag` 1, or the done flag is up, raises the done flag, hides the
 * SEG29:0x1210 port and releases the SEG29:0x11ea window.  Returns the done flag.  The five
 * module helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from check_timer_tick: the Mac build's build_check(short), bound by call-graph agreement (8 vs 4) and by call-sequence alignment (15.5 aligned gaps in build_map, compute_moves, do_rivers leave it as the lone unbound callee, none against); the arity agrees.
 */

#include "colonization.h"

#include "classes.h"
extern int g_opening_dismissed;                         /* DGROUP:0x1d04 */

extern int g_stub_a;

extern void show_ship(int n);

extern void MSHideWindow_of_1040_a80c(void *d);
extern void win_release_1040_acac(void *w);

int build_check(int flag)
{
    long now;

    now = MSGetTicks_1040_aaa0();
    if (g_tick_last >= 0) {
        if (now - g_tick_last < 600L)
            goto skip;
    }
    if (g_tick_index >= 10) {
        g_tick_done = 1;
        g_tick_last = now;
    } else {
        show_ship(g_tick_index);
        g_tick_index++;
        g_tick_last = now;
    }
skip:
    MSDoAllTasks_1040_ac99();
    if ((g_opening_dismissed == 1 && flag == 1) || g_tick_done == 1) {
        g_tick_done = 1;
        MSHideWindow_of_1040_a80c((DialogWin *)((char *)&g_opening_win + 0x26));
        MSDoAllTasks_1040_ac99();
        win_release_1040_acac(&g_opening_win);
    }
    return g_tick_done;
}
