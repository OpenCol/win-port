/* keyboard_accelerator_handler -- shipped at 1048:d006.
 *
 * Ported by tools/port.py from matched/game/keyboard_accelerator_handler.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_in_accel;                              DGROUP:0x2012
 *   extern int g_cheat_step;                             DGROUP:0x4cf6
 *   extern int far g_quit_flag;                          DGROUP:0x4b04
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *   extern char far g_game_record2;                      SEG20:0x40d6
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_game_in_progress;                   SEG20:0x77c4
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern char far g_pedia_ctx;                         SEG20:0x387c
 *   extern char far g_report_window;                     SEG20:0x1d06
 *   extern char far g_menu_owner;                        SEG20:0x0656
 *   extern char far g_map_window;                        SEG20:0x0658
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern char far g_europe_win;                        SEG20:0x2fd2
 *   extern int far g_view_mode;                          SEG20:0x7792
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ask_text_resource(char *name);
col_i16 get_w_6_1048_e56c(void *r);
void cheat_menu_toggle(void);
void european_status(void *p, col_i16 a, col_i16 b);
void menu_enable_item_1048_e2dd(void *m, col_i32 id, col_i16 on);

#include "gameflag.h"

















extern void open_overlay_window(void);
extern void hall_of_fame_entry(void);
extern void close_overlay_window(void);
extern unsigned char map_plane3_at(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern void open_colonopedia(void *p, col_i16 kind, col_i16 n);
extern void ReportWin_DoReport(void *w, col_i16 n, col_i16 a);
extern void MapWin_RevealHiddenTerrain(void *w);
extern void MapWin_Recenter(void *w, col_i16 x, col_i16 y);
extern void view_pieces(void *rec);
extern void map_goto(col_i16 x, col_i16 y, col_i16 n);
extern void select_next_unit(void *rec, col_i16 a);
extern col_i16 MapWin_ZoomIn(void *w);
extern col_i16 MapWin_ZoomOut(void *w);
extern void orders_key_command(void *rec, col_i16 key);

/* same object: 4-byte `push cs; call ` (rule 47) */






void keyboard_accelerator_handler(col_i16 key)
{
    col_i16 z;

    if (g_in_accel != 0)
        return;
    g_in_accel = 1;
    if (get_w_6_1048_e56c(&(*g_game_record2)) == 0)
        goto done;
    if (key == 0x257)
        g_cheat_step = 1;
    else if (key == 0x249 && g_cheat_step == 1)
        g_cheat_step = 2;
    else if (key == 0x24e && g_cheat_step == 2)
        cheat_menu_toggle();
    else
        g_cheat_step = 0;
    switch (key) {
    case 0x251:
        if (ask_text_resource("DOS") == 2)
            g_quit_flag = 1;
        break;
    case 0x252:
        if (ask_text_resource("RETIRE") == 2) {
            open_overlay_window();
            if ((g_game_flags & GAMEFLAG_SCORED) == 0)
                hall_of_fame_entry();
            g_game_in_progress = 0;
            g_quit_flag = 1;
            close_overlay_window();
        }
        break;
    case 0xb0:
        if (g_view_nation >= 4
            || (map_plane3_at(g_cursor_x, g_cursor_y) & (0x10 << g_view_nation)) != 0
            || g_reveal_all != 0)
            open_colonopedia(&(*g_pedia_ctx), 2, terrain_at(g_cursor_x, g_cursor_y));
        break;
    case 0xb1:
        ReportWin_DoReport(&g_report_window, 2, 0);
        break;
    case 0xb2:
        ReportWin_DoReport(&g_report_window, 3, 0);
        break;
    case 0xb3:
        ReportWin_DoReport(&g_report_window, 4, 0);
        break;
    case 0xb4:
        ReportWin_DoReport(&g_report_window, 5, 0);
        break;
    case 0xb5:
        ReportWin_DoReport(&g_report_window, 6, 0);
        break;
    case 0xb6:
        ReportWin_DoReport(&g_report_window, 7, 0);
        break;
    case 0xb7:
        ReportWin_DoReport(&g_report_window, 8, 0);
        break;
    case 0xb8:
        ReportWin_DoReport(&g_report_window, 1, 0);
        break;
    case 0xb9:
        ReportWin_DoReport(&g_report_window, 9, 0);
        break;
    case 'H':
        MapWin_RevealHiddenTerrain(&g_map_window);
        break;
    case 'E':
        european_status(&g_europe_win, g_player_nation, -1);
        break;
    case 'C':
        MapWin_Recenter(&g_map_window, g_cursor_x, g_cursor_y);
        break;
    case 'V':
        view_pieces(&(*g_game_record2));
        break;
    case 'M':
        if (g_view_mode == 0)
            map_goto(g_cursor_x, g_cursor_y, 1);
        else
            select_next_unit(&(*g_game_record2), 0);
        break;
    case 'Z':
        z = MapWin_ZoomIn(&g_map_window);
        if (z == 4)
            menu_enable_item_1048_e2dd(&g_menu_owner, 0x130L, 0);
        else if (z == -3)
            menu_enable_item_1048_e2dd(&g_menu_owner, 0x131L, 1);
        break;
    case 'X':
        z = MapWin_ZoomOut(&g_map_window);
        if (z == -4)
            menu_enable_item_1048_e2dd(&g_menu_owner, 0x131L, 0);
        else if (z == 3)
            menu_enable_item_1048_e2dd(&g_menu_owner, 0x130L, 1);
        break;
    case 0x41: case 0x50: case 0x42: case 0x44: case 0x46: case 0x52: case 0x53: case 0x54:
    case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa6: case 0xa7: case 0xa8:
    case 0xba: case 0xc2: case 0xc3: case 0xd0: case 0xd1:
    default:
        orders_key_command(&(*g_game_record2), key);
        break;
    }
done:
    g_in_accel = 0;
}
