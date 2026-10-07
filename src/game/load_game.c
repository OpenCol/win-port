/* PORT: this file's own declarations of GameBrains_Reset, MainWin_ResetWindows removed: the header declares them from the definition (W1 step 3). */
/* load_game: candidate for 1008:8543 (179 bytes).  Verdict: RELOCATABLE.
 *
 * The Load Game command: with a game in progress (SEG20:0x77c4) the report window is closed
 * and "LOADOVERGAME" asked -- answer 2 cancels. Then the current slot (DGROUP:0xc20) is reset,
 * the open dialog ("Saved Game") run and the chosen file loaded through load_game_file;
 * a clean load runs MainWin_ResetWindows on the game record, sets the byte at DGROUP:0x8b and
 * GameBrains_Reset. Returns the loader's status.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
#include "classes.h"

extern MainWin g_main_win;       /* SEG20:0x5ca */
extern GameBrains g_game_brains;      /* SEG20:0x40d6 */

extern int load_game_file(char *path);
/* same object: 4-byte `push cs; call ` (rule 47) */
int load_game(void)
{
    char path[0x100];
    int err;

    if (g_game.game_in_progress != 0) {
        ReportWin_Done(&g_report_window);
        if (show_text_dialog_1008_cdf3("LOADOVERGAME") == 2)
            return 0;
    }
    g_new_building = -1;
    path[0] = 0;
    if (file_open_dialog(path, "Saved Game\0*.sav\0", 1)) {
        err = load_game_file(path);
        if (err == 0) {
            MainWin_ResetWindows(&g_main_win);
            g_loaded_flag = 1;
            GameBrains_Reset(&g_game_brains);
        }
    }
    return err;
}
