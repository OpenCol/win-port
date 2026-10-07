/* gui_init: candidate for 1070:af5c (43 bytes).  Verdict: RELOCATABLE.
 * size-source: 43. The last 8 are a second, unreachable epilogue.
 * name-source: descriptive
 *
 * Stores the instance and previous-instance handles at DGROUP:0x56e2 and 0x56e4, calls
 * write_gui_error_log (whose two reporters are empty in this build, so no log is opened) and
 * platform_startup (window classes for the first instance only, the MSMMWindow, WinG.dll, the
 * random seed), and answers 1.
 *
 * `gui_shutdown` names it from the other end: it is "the path that also calls
 * 1070:af5c and run_game".
 */

#include "colonization.h"

extern void platform_startup(void);

int gui_init(int hinst, int hprev)
{
    g_hinstance = hinst;
    g_hprev = hprev;
    write_gui_error_log();
    platform_startup();
    return 1;
}
