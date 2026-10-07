/* PORT: this file's own declarations of MainWin_ResetWindows removed: the header declares it from the definition (W1 step 3). */
/* load_game_from_dialog: candidate for 1028:e2f8 (88 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Asks for a "Saved Game" file and loads it: `file_open_dialog` -- which is
 * `file_dialog(a, b, 0, d, 0)`, the OPEN variant, so this is a load and not a save -- then
 * `load_game_file` on the chosen path and `MainWin_ResetWindows` on the game record at SEG20:0x5ca,
 * which `load_game`'s header already calls the clean-load refresh.
 *
 * It returns 1 whatever happens. The slot is initialised to 1 before the dialog and never
 * written again; a cancelled dialog skips both calls and still reports success.
 *
 * The path buffer is 0x100 bytes and the frame is 0x102, so the only other local is the
 * result.
 */

#include "colonization.h"

#include "classes.h"
extern MainWin g_main_win;   /* SEG20:0x5ca */

extern int load_game_file(char *path);

int load_game_from_dialog(void)
{
    char path[0x100];
    int ok;

    ok = 1;
    if (file_open_dialog(path, "Saved Game\0*.sav", 1) != 0) {
        load_game_file(path);
        MainWin_ResetWindows(&g_main_win);
    }
    return ok;
}
