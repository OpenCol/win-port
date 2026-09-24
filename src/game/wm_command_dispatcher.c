/* wm_command_dispatcher -- shipped at 1048:c0b2.
 *
 * Ported by tools/port.py from matched/game/wm_command_dispatcher.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_menu_owner;                        SEG20:0x0656
 *   extern char far g_map_window;                        SEG20:0x0658
 *   extern char far g_win_67e;                           SEG20:0x067e
 *   extern char far g_map_overlay;                       SEG20:0x0de0
 *   extern char far g_win_e06;                           SEG20:0x0e06
 *   extern char far g_info_win;                          SEG20:0x1566
 *   extern char far g_win_158c;                          SEG20:0x158c
 *   extern int far g_text_value;                         DGROUP:0x251a
 *   extern char far g_game_record2;                      SEG20:0x40d6
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_view_override;                      SEG20:0x77a6
 *   extern int far g_game_in_progress;                   SEG20:0x77c4
 *   extern int far g_rebel_sentiment;                    SEG20:0x77d2
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_text_args;                          SEG31:0x02d4
 *   extern int far g_quit_flag;                          DGROUP:0x4b04
 *   extern char far g_4b2c;                              DGROUP:0x4b2c
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ask_text_resource(char *name);
col_i16 get_w_6_1048_e56c(void *r);
void MSHideWindow_of_1048_d4d5(void *o);
void begin_trade_route(void);
void close_dialog_at_26_1048_dd42(void *w);
void create_unit(void);
void debug_info_flags(void);
void european_status(void *p, col_i16 a, col_i16 b);
void find_colony(void);
void game_colony_options(void);
void get_game_options(void);
void jukebox_by_song(void);
void jukebox_control_panel(void);
void kill_indians(void);
void memory_check(void);
void menu_call_da9f(void *m, col_i32 a, char *text);
void menu_enable_item_1048_e2dd(void *m, col_u32 id, char enable);
void return_to_europe(void);
void set_human_player(void);
void show_colony_sites(void);
void show_strategy(void);
void sound_options(void);
void test_routine(void);

#include "gameflag.h"























extern char * current_window(void);
extern void format_save_filename(char *dst);

extern void save_game_to_file(char *path);
extern col_i16 load_game(void);
extern void declare_independence(void);
extern void hall_of_fame_entry(void);
extern void map_goto(col_i16 x, col_i16 y, col_i16 flag);
extern void select_next_unit(char *ctx, col_i16 n);
extern void view_pieces(void *ctx);
extern col_i16 MapWin_ZoomIn(void *w);
extern col_i16 MapWin_ZoomOut(void *w);
extern void MapWin_SetZoom(void *w, col_i16 z);
extern void MapWin_RevealHiddenTerrain(void *w);
extern void MapWin_Recenter(void *o, col_i16 mx, col_i16 my);
extern void orders_key_command(void *rec, col_i16 key);
extern void sub_1008_3cc8(void *ctx);
extern unsigned char map_plane3_at(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern void open_colonopedia(void *p, col_i16 kind, col_i16 n);
extern void ReportWin_DoReport(void *w, col_i16 n, col_i16 a);
extern void route_edit(col_i16 r);
extern void route_create(void);
extern void route_delete(void);
extern void about_colonization(void);
extern void spanish_succession_event(void);
extern void declaration_of_independence(void);
extern void intervention(void);
extern void redraw_map_1040_11b7(col_i16 a);
extern void window_relayout(void *w);
extern col_i16 prompt_text_number(char *ctx, char *name, col_i16 n);
extern void play_sound_effect(col_i16 id);
extern void debug_picture_viewer(void);
extern void open_overlay_window(void);
extern void close_overlay_window(void);
extern void default_desktop(void *o);
extern col_i16 file_save_dialog(void *a, void *b, void *c, col_i16 d);

/* same object: 4-byte `push cs; call ` (rule 47) */
























void wm_command_dispatcher(col_i32 cmd)
{
    char *w;
    char *p;
    col_i16 zoom;
    col_i16 terrain;
    col_i16 n;
    col_i16 ans;
    char path[256];
    char date[80];
    char msg[80];
    char filter[52] = "Saved Game\0" "2345678.SAV\0" "Previously Saved Game\0" "*.sav\0";

    if (get_w_6_1048_e56c(&(*g_game_record2)) == 0)
        goto out;
    w = current_window();
    switch (cmd) {
    case 55:
        get_game_options();
        break;
    case 100:
        game_colony_options();
        break;
    case 101:
        sound_options();
        break;
    case 102:
        jukebox_by_song();
        break;
    case 103:
        jukebox_control_panel();
        break;
    case 104:
        format_save_filename(date);
        msg[0] = 0;
        p = &filter[11];
        strcpy(p, date);
        path[0] = 0;
        if (file_save_dialog(path, filter, "sav", 0))
            save_game_to_file(path);
        break;
    case 105:
        load_game();
        break;
    case 106:
        declare_independence();
        break;
    case 107:
        if (ask_text_resource("RETIRE") == 2) {
            open_overlay_window();
            if (!(g_game_flags & GAMEFLAG_SCORED))
                hall_of_fame_entry();
            g_game_in_progress = 0;
            g_quit_flag = 1;
            close_overlay_window();
        }
        break;
    case 108:
        if (ask_text_resource("DOS") == 2)
            g_quit_flag = 1;
        break;
    case 300:
        if (g_view_mode == 0)
            map_goto(g_cursor_x, g_cursor_y, 1);
        else
            select_next_unit(&(*g_game_record2), 0);
        break;
    case 301:
        view_pieces(&(*g_game_record2));
        break;
    case 302:
        european_status((char *)w + 0x2a08, g_player_nation, -1);
        break;
    case 303:
        find_colony();
        break;
    case 304:
        zoom = MapWin_ZoomIn((char *)w + 0x8e);
        if (zoom == 4)
            menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x130L, 0);
        else if (zoom == -3)
            menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x131L, 1);
        break;
    case 305:
        zoom = MapWin_ZoomOut((char *)w + 0x8e);
        if (zoom == -4)
            menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x131L, 0);
        else if (zoom == 3)
            menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x130L, 1);
        break;
    case 306:
        MapWin_SetZoom((char *)w + 0x8e, 4);
        zoom = 4;
        menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x130L, 0);
        menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x131L, 1);
        break;
    case 307:
        MapWin_SetZoom((char *)w + 0x8e, 0);
        zoom = 0;
        menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x130L, 1);
        menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x131L, 1);
        break;
    case 308:
        MapWin_SetZoom((char *)w + 0x8e, -4);
        zoom = -4;
        menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x130L, 1);
        menu_enable_item_1048_e2dd((char *)w + 0x8c, 0x131L, 1);
        break;
    case 310:
        MapWin_RevealHiddenTerrain((char *)w + 0x8e);
        break;
    case 311:
        MapWin_Recenter((char *)w + 0x8e, g_cursor_x, g_cursor_y);
        break;
    case 312:
        default_desktop(w);
        break;
    case 400:
        orders_key_command(&(*g_game_record2), 0x41);
        break;
    case 401:
        orders_key_command(&(*g_game_record2), 0x57);
        break;
    case 402:
        orders_key_command(&(*g_game_record2), 0x46);
        break;
    case 404:
        orders_key_command(&(*g_game_record2), 0x53);
        break;
    case 405:
        orders_key_command(&(*g_game_record2), 0x42);
        break;
    case 406:
        orders_key_command(&(*g_game_record2), 0x50);
        break;
    case 407:
        orders_key_command(&(*g_game_record2), 0x52);
        break;
    case 408:
        orders_key_command(&(*g_game_record2), 0x4c);
        break;
    case 409:
        orders_key_command(&(*g_game_record2), 0x55);
        break;
    case 411:
        orders_key_command(&(*g_game_record2), 0x47);
        break;
    case 412:
        begin_trade_route();
        sub_1008_3cc8(&(*g_game_record2));
        break;
    case 413:
        return_to_europe();
        sub_1008_3cc8(&(*g_game_record2));
        break;
    case 414:
        orders_key_command(&(*g_game_record2), 0xd1);
        break;
    case 415:
        orders_key_command(&(*g_game_record2), 0x4f);
        break;
    case 416:
        orders_key_command(&(*g_game_record2), 0x144);
        break;
    case 500:
        if (g_view_nation >= 4
            || (map_plane3_at(g_cursor_x, g_cursor_y) & (0x10 << g_view_nation))
            || g_reveal_all != 0) {
            terrain = terrain_at(g_cursor_x, g_cursor_y);
            open_colonopedia((char *)w + 0x32b2, 2, terrain);
        }
        break;
    case 501:
        ReportWin_DoReport((char *)w + 0x173c, 2, 0);
        break;
    case 502:
        ReportWin_DoReport((char *)w + 0x173c, 3, 0);
        break;
    case 503:
        ReportWin_DoReport((char *)w + 0x173c, 4, 0);
        break;
    case 504:
        ReportWin_DoReport((char *)w + 0x173c, 5, 0);
        break;
    case 505:
        ReportWin_DoReport((char *)w + 0x173c, 6, 0);
        break;
    case 506:
        ReportWin_DoReport((char *)w + 0x173c, 7, 0);
        break;
    case 507:
        ReportWin_DoReport((char *)w + 0x173c, 8, 0);
        break;
    case 508:
        ReportWin_DoReport((char *)w + 0x173c, 1, 0);
        break;
    case 509:
        ReportWin_DoReport((char *)w + 0x173c, 9, 0);
        break;
    case 600:
        route_edit(-1);
        break;
    case 601:
        route_create();
        break;
    case 602:
        route_delete();
        break;
    case 700:
        open_colonopedia((char *)w + 0x32b2, 0, -1);
        break;
    case 701:
        open_colonopedia((char *)w + 0x32b2, 1, -1);
        break;
    case 702:
        open_colonopedia((char *)w + 0x32b2, 2, -1);
        break;
    case 703:
        open_colonopedia((char *)w + 0x32b2, 3, -1);
        break;
    case 704:
        open_colonopedia((char *)w + 0x32b2, 4, -1);
        break;
    case 705:
        open_colonopedia((char *)w + 0x32b2, 5, -1);
        break;
    case 706:
        about_colonization();
        break;
    case 800:
        create_unit();
        break;
    case 801:
        debug_info_flags();
        break;
    case 802:
        if (g_reveal_all != 0 || g_view_override >= 0)
            n = 6;
        else
            n = 5;
        g_text_value = 3;
        ans = ask_text_resource("SETVIEW");
        g_reveal_all = ans == 5;
        g_view_override = ans < 5 ? ans - 1 : -1;
        if (g_reveal_all != 0)
            g_game_flags &= 0x7fff;
        if (g_view_override >= 0)
            g_view_nation = g_view_override;
        redraw_map_1040_11b7(1);
        window_relayout(&(*g_map_overlay));
        break;
    case 803:
        set_human_player();
        break;
    case 804:
        kill_indians();
        break;
    case 805:
        if (g_rebel_sentiment < 0x4b) {
            g_rebel_sentiment = 0x4b;
            if (g_mother_country_idx < 0)
                spanish_succession_event();
        } else if (!(g_game_flags & GAMEFLAG_INDEPENDENCE))
            declaration_of_independence();
        else if (!(g_game_flags & GAMEFLAG_INTERVENTION))
            intervention();
        else {
            g_game_flags |= 0x20;
            ask_text_resource("FORCED");
        }
        break;
    case 806:
        if (prompt_text_number("DEBUG", "SOUND", 1) == 0)
            play_sound_effect((*g_text_args));
        break;
    case 807:
        memory_check();
        break;
    case 808:
        show_strategy();
        break;
    case 809:
        show_colony_sites();
        break;
    case 810:
        test_routine();
        break;
    case 811:
        debug_picture_viewer();
        break;
    case 1000:
        if (!((col_i16)g_4b2c & 1)) {
            close_dialog_at_26_1048_dd42(&g_map_window);
            menu_call_da9f(&g_menu_owner, 0x3e8L, "Hide Map");
            g_4b2c |= 1;
        } else {
            MSHideWindow_of_1048_d4d5(&g_win_67e);
            menu_call_da9f(&g_menu_owner, 0x3e8L, "Show Map");
            g_4b2c &= ~1;
        }
        break;
    case 1001:
        if (!((col_i16)g_4b2c & 2)) {
            close_dialog_at_26_1048_dd42(&(*g_map_overlay));
            menu_call_da9f(&g_menu_owner, 0x3e9L, "Hide World");
            g_4b2c |= 2;
        } else {
            MSHideWindow_of_1048_d4d5(&g_win_e06);
            menu_call_da9f(&g_menu_owner, 0x3e9L, "Show World");
            g_4b2c &= ~2;
        }
        break;
    case 1002:
        if (!((col_i16)g_4b2c & 4)) {
            close_dialog_at_26_1048_dd42(&(*g_info_win));
            menu_call_da9f(&g_menu_owner, 0x3eaL, "Hide Status");
            g_4b2c |= 4;
        } else {
            MSHideWindow_of_1048_d4d5(&g_win_158c);
            menu_call_da9f(&g_menu_owner, 0x3eaL, "Show Status");
            g_4b2c &= ~4;
        }
        break;
    }
out:
    ;
}
