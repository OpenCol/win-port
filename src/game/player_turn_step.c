/* player_turn_step -- shipped at 1008:3145.
 *
 * Ported by tools/port.py from matched/game/player_turn_step.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_view_override;                      SEG20:0x77a6
 *   extern unsigned char far g_flags_7785;               SEG20:0x7785
 *   extern int far g_message_flag;                       SEG20:0x77c6
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int far g_game_in_progress;                   SEG20:0x77c4
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_message_pending;                    SEG20:0x77c8
 *   extern char far g_map_overlay;                       SEG20:0x0de0
 *   extern char far g_info_win;                          SEG20:0x1566
 *   extern char far g_loaded_flag;                       DGROUP:0x008b
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *   extern int g_cycle_flag;                             DGROUP:0x0076
 *   extern int g_blink;                                  DGROUP:0x0078
 *   extern unsigned long g_last_blink;                   DGROUP:0x007a
 *   extern int g_4b00;                                   DGROUP:0x4b00
 *   extern int g_4b02;                                   DGROUP:0x4b02
 *
 * The file's own Ctx dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 select_next_unit(Ctx *c, col_i16 a);
col_u32 MSGetTicks_1008_cee5(void);
void autosave_to_slot(void);
void peek_message_only_1008_cefc(void);
void refresh_unit_cursor(Ctx *c);

extern void window_relayout(void *rec);
extern void set_current_nation(col_i16 n);
extern void offer_mercenaries(void);
extern col_i16 unit_selectable_or_in_europe(col_i16 u);
extern void music_pick_track(void);
extern col_i16 in_map_rect(col_i16 x, col_i16 y);
extern void map_copy_tiles(col_i16 x, col_i16 y, col_i16 w, col_i16 h);
extern void map_blit_tiles(col_i16 x, col_i16 y, col_i16 w, col_i16 h);
extern void blink_overview_unit(void *rec, col_i16 on);

/* 5-byte NOP form, so extern (rule 47) */
extern void arrived_unit_act(Ctx *c);

/* same object: 4-byte `push cs; call ` (rule 47) */






void player_turn_step(Ctx *c)
{
    switch (c->phase) {
    case 100:
        g_view_nation = c->nation;
        g_cycle_flag = 0;
        if (g_view_override >= 0)
            g_view_nation = g_view_override;
        window_relayout(&(*g_info_win));
        if (g_loaded_flag == 0 && (g_flags_7785 & 4))
            autosave_to_slot();
        g_message_flag = 1;
        g_selected_unit = -1;
        g_4b00 = 0;
        select_next_unit(c, 0);
        c->phase = 0;
        g_4b02 = 0;
        set_current_nation(g_view_nation);
        offer_mercenaries();
        break;
    case 0:
        if (g_message_flag != 0 && g_game_in_progress != 0) {
            c->f0a = 0;
            if (g_view_mode == 0 && unit_selectable_or_in_europe(g_selected_unit) == 0)
                c->f0a = select_next_unit(c, 0);
            if (g_message_flag != 0) {
                if (c->f0a == 0)
                    window_relayout(&(*g_info_win));
                if (g_view_mode == 0)
                    arrived_unit_act(c);
                else
                    c->phase = 1;
            }
        } else {
            c->phase = 3;
        }
        g_4b02 = 0;
        break;
    case 1:
        music_pick_track();
        if (g_4b02 != 0) {
            c->phase = 0;
        } else if (g_message_flag != 0) {
            if (g_view_mode == 0)
                g_4b00 = 1;
            if ((col_i32)MSGetTicks_1008_cee5() > (col_i32)(g_last_blink + 25)) {
                g_last_blink = MSGetTicks_1008_cee5();
                g_cycle_flag = !g_cycle_flag;
                if (g_message_pending != 0)
                    window_relayout(&(*g_info_win));
                if (in_map_rect(g_cursor_x, g_cursor_y)) {
                    map_copy_tiles(g_cursor_x, g_cursor_y, 1, 1);
                    refresh_unit_cursor(c);
                    blink_overview_unit(&(*g_map_overlay), g_blink = !g_blink);
                    map_blit_tiles(g_cursor_x, g_cursor_y, 1, 1);
                } else {
                    blink_overview_unit(&(*g_map_overlay), g_blink = !g_blink);
                }
            }
        } else {
            c->phase = 3;
        }
        break;
    case 2:
        c->phase = 0;
        peek_message_only_1008_cefc();
        break;
    case 3:
        c->f6 = 0;
        g_loaded_flag = 0;
        g_message_pending = 1;
        break;
    }
}
