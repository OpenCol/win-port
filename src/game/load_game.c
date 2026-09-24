/* load_game -- shipped at 1008:8543.
 *
 * Ported by tools/port.py from matched/game/load_game.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_game_in_progress;                   SEG20:0x77c4
 *   extern char far g_report_window;                     SEG20:0x1d06
 *   extern char far g_game_record;                       SEG20:0x05ca
 *   extern char far g_game_record2;                      SEG20:0x40d6
 *   extern int far g_new_building;                       DGROUP:0x0c20
 *   extern char g_loaded_flag;                           DGROUP:0x008b
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 show_text_dialog_1008_cdf3(char *msg);
void set_defaults(void *rec);

extern void close_report_window(void *w);

extern col_i16 file_open_dialog(void *path, void *title, col_i16 d);
extern col_i16 load_saved_game(char *path);
extern void reset_windows_for_new_game(void *rec);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 load_game(void)
{
    char path[0x100];
    col_i16 ok;

    if (g_game_in_progress != 0) {
        close_report_window(&g_report_window);
        if (show_text_dialog_1008_cdf3("LOADOVERGAME") == 2)
            return 0;
    }
    g_new_building = -1;
    path[0] = 0;
    if (file_open_dialog(path, "Saved Game", 1)) {
        ok = load_saved_game(path);
        if (ok == 0) {
            reset_windows_for_new_game(&g_game_record);
            g_loaded_flag = 1;
            set_defaults(&(*g_game_record2));
        }
    }
    return ok;
}
