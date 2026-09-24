/* reset_windows_for_new_game -- shipped at 1048:a896.
 *
 * Ported by tools/port.py from matched/game/reset_windows_for_new_game.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void cheat_menu_toggle(void);
void close_dialog_1048_d8f7(void *w);

#include "gameflag.h"
typedef struct {
    char pad[0x8e];
    char map[0xb4 - 0x8e]; /* +0x8e */
    char dialog[0x816 - 0xb4]; /* +0xb4 */
    char w816[0xf9c - 0x816]; /* +0x816 */
    char w9c[0x173c - 0xf9c]; /* +0xf9c */
    char report[0x32b2 - 0x173c]; /* +0x173c */
    char w32b2[2]; /* +0x32b2 */
} Game;



extern void MapWin_Recenter(void *o, col_i16 mx, col_i16 my);
extern void window_relayout(void *w);
extern void empty_1060_08cf(void *w, col_i16 a);
extern void win_close_1060_2a47(void *w);
extern void close_report_window(void *o);

/* same object: 4-byte `push cs; call ` */



void reset_windows_for_new_game(Game *g)
{
    MapWin_Recenter(g->map, g_cursor_x, g_cursor_y);
    window_relayout(g->map);
    window_relayout(g->w9c);
    empty_1060_08cf(g->w816, 3);
    window_relayout(g->w816);
    win_close_1060_2a47(g->w32b2);
    close_report_window(g->report);
    close_dialog_1048_d8f7(g->dialog);
    g_game_flags ^= GAMEFLAG_CHEAT_MENU;
    cheat_menu_toggle();
}
