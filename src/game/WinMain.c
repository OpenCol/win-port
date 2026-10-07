/* WinMain: candidate for 1008:cbff (67 bytes).  Verdict: RELOCATABLE.
 *
 * The program's WinMain (, 10 bytes of arguments): with no previous instance it
 * initialises the widget layer, opens the data DLLs and runs; otherwise it shows
 * "We've had bad luck with multiple\ninstances, so just forget it, alright?" / "Fat Chance Buddy" and leaves. Returns 0 either way.
 *
 * RENAMED 2026-09-26 from refuse_second_instance: The program's WinMain (, called from entry): with no previous instance it runs the whole game -- gui_init, open_coldata_dlls, gui_shutdown -- and only in the else branch shows the 'Fat Chance Buddy' box. The name describes the minor refusal branch rather than the function's job of starting and running the program.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void run_game(void);

int WinMain(unsigned hinst, unsigned hprev, char *cmdline, int show)
{
    if (hprev == 0) {
        gui_init(hinst, hprev);
        run_game();
        gui_shutdown();
    } else {
        MessageBox(0, "We've had bad luck with multiple\ninstances, so just forget it, alright?", "Fat Chance Buddy", 0x30);
    }
    return 0;
}
