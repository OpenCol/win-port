/* gui_shutdown: candidate for 1070:af87 (71 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: gui_shutdown
 *
 * Tears the widget layer down: `gui_teardown` -- which calls `spin_over_count`,
 * `free_wing_library` and `destroy_MSMMWindow_window` -- and then destroys the global
 * timer object at DGROUP:0x2ae8 if there is one. Returns 1 always.
 *
 * COMPILED AS C++, and the `delete` is why. The shipped bytes push the timer pointer and
 * the literal 3 into `MrTimer_dtor` and adjust the unwinder's constructed-object counter at
 * DGROUP:0x10 around it -- that whole sequence is what Borland emits for `delete p` on a
 * class with a destructor, flag bit 0 meaning "free the storage too". None of it is written
 * in the source; `delete g_timer;` is the entire statement. Written in C as an explicit
 * `MrTimer_dtor(g_timer, 3)` the counter adjustment is missing and the function is short.
 *
 * THE RECORDED PURPOSE FOR THIS ADDRESS WAS WRONG, and wrong in a way worth keeping.
 * `re/functions/1070_af87.json` read "Constructor of some C++ class: it increments the
 * framework's live-object counter", derived from the counter instruction alone. The
 * instruction is there and the inference from it is backwards: the counter moves around a
 * `delete` as well as around a construction, and this function destroys.
 *
 * THE CALL SITE IS ODD AND IS NOT EXPLAINED HERE. Its only caller is
 * `WinMain`, which calls it on the path taken when `[bp+0xc]` is ZERO --
 * the path that also calls `gui_init` and `run_game`, and the path a name
 * like that implies is the FIRST instance's. A first instance tearing the widget layer
 * down immediately after opening the data DLLs does not read as startup. The consistent
 * reading is that the caller is an exit path and its argument records whether this instance
 * was the one that loaded the shared library -- only that instance frees it -- but that is
 * a reading of one branch, not something these bytes establish, and the caller's name has
 * not been changed on the strength of it.
 */

#include "colonization.h"

#include "classes.h"

extern MrTimer *g_timer;   /* DGROUP:0x2ae8, declared */

extern "C" int gui_shutdown(void)
{
    gui_teardown();
    if (g_timer != 0)
        delete g_timer;
    return 1;
}
