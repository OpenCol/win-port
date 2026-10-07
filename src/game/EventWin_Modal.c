/* EventWin_Modal: candidate for 1060:9f50 (93 bytes).  Verdict: RELOCATABLE.
 * symbol: EventWin_Modal
 * name-source: sibling-build
 *
 * Runs the object modally: its running flag (+0x76) set and the object made
 * the modal window (DGROUP:0x2ae4), disable_parent on its window, the message
 * pump (MSDoTask_1060_add3) until the flag drops, then enable_parent.  The two
 * helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from run_modal: the Mac build's EventWin::Modal(void): this only, setting the running flag (+0x76), making the window the modal one and pumping until it clears. The width matches.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

extern char g_stub_s[];
extern int g_stub_a;
extern void disable_parent(void *win);
extern void enable_parent(void *win);

extern void *get_win_1060_ab6e(void *o);

void EventWin_Modal(EventWin *o)
{
    o->running = 1;
    g_modal_win = o;
    disable_parent((MSWin *)(get_win_1060_ab6e(o)));
    while (o->running != 0)
        MSDoTask_1060_add3();
    enable_parent((MSWin *)(get_win_1060_ab6e(o)));
}
