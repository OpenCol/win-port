/* timer_window_proc: candidate for 1078:1968 (134 bytes).  Verdict: RELOCATABLE.
 *
 * Window procedure of the timer daemon window: WM_TIMER goes to timer_tick_slot with the
 * object from window long 0 (and returns 0); everything else, and every message while
 * that long is still null, goes to DefWindowProc.
 */

#include "colonization.h"

#include "w16const.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
#include "records.h"
#include "classes.h"
extern void timer_tick_slot(MrTimer *timer, int i);
long timer_window_proc(unsigned hwnd, unsigned msg, unsigned wparam, long lparam)
{
    void *o = (void *)GetWindowLong(hwnd, 0);

    if (o == 0)
        return DefWindowProc(hwnd, msg, wparam, lparam);
    switch (msg) {
    case WM_TIMER:
        timer_tick_slot((MrTimer *)o, wparam);
        break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
