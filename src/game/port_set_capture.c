/* port_set_capture: candidate for 1048:daa5 (37 bytes).  Verdict: RELOCATABLE.
 * symbol: port_set_capture
 *
 * win_set_capture on the window get_win_1048_e6d0 reads from the port (its pointer at +6) (the
 * latter -called and defined in-file as a stub).
 *
 * RENAMED 2026-09-26 from port_call_2d24: win_set_capture on the window the port belongs to.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void win_set_capture(void *win);

extern void *get_win_1048_e6d0(void *w);

void port_set_capture(Screen *w)
{
    win_set_capture((MSWin *)(get_win_1048_e6d0(w)));
}
