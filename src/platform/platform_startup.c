/* platform_startup: candidate for 1070:b0f9 (40 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Start-up: registers the widget library's window classes -- but only when `g_hprev`, the
 * previous-instance handle at DGROUP:0x56e4, is zero, which is Windows' rule that only the
 * FIRST running copy registers classes -- then creates the multimedia window, loads WinG.dll and seeds the random number generator from the clock.
 *
 * The first three are calls, so extern; `seed_random_from_clock` is the four-byte
 * `push cs; call ` with no NOP, so it is defined here (rule 47).
 *
 * RENAMED 2026-09-26 from register_window_classes: The 40-byte start-up hub gui_init calls. It calls register_ms_classes only when g_hprev is 0 (the first instance), and it always creates the MSMMWindow, loads the resource library and seeds the RNG from the clock. The class registration is done by register_ms_classes, and a second instance registers nothing here, so the name points at a callee's job and hides the other three steps.
 */

#include "colonization.h"

extern void load_wing_library(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

void platform_startup(void)
{
    if (g_hprev == 0)
        register_ms_classes();
    create_MSMMWindow_window();
    load_wing_library();
    seed_random_from_clock();
}
