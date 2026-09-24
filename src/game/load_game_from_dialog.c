/* load_game_from_dialog -- shipped at 1028:e2f8.
 *
 * Ported by tools/port.py from matched/game/load_game_from_dialog.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_game_record;                       SEG20:0x05ca
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 file_open_dialog(void *path, void *title, col_i16 d);
extern void load_saved_game(char *path);
extern void reset_windows_for_new_game(void *rec);

col_i16 load_game_from_dialog(void)
{
    char path[0x100];
    col_i16 ok;

    ok = 1;
    if (file_open_dialog(path, "Saved Game", 1) != 0) {
        load_saved_game(path);
        reset_windows_for_new_game(&g_game_record);
    }
    return ok;
}
