/* orders_key_command -- shipped at 1008:37b8.
 *
 * Ported by tools/port.py from matched/game/orders_key_command.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern char far g_info_win;                          SEG20:0x1566
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern unsigned char far g_view_nation;              SEG20:0x7798
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_message_flag;                       SEG20:0x77c6
 *   extern int far g_message_pending;                    SEG20:0x77c8
 *   extern unsigned char far g_order_flags[];            SEG30:0x0050
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *
 * The file's own ArrivedWin, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 arrived_landfall(ArrivedWin *o, col_i16 n);
col_i16 arrived_unit_orders_1(void);
col_i16 build_colony(void);
col_i16 build_road(void);
col_i16 disband_unit(void);
col_i16 fortify_unit(void);
col_i16 open_colony_under_unit_1008_0d7f(void);
col_i16 open_colony_under_unit_1008_0e05(void);
col_i16 orders_port(void);
col_i16 peek_message_only_1008_cefc(void);
col_i16 pick_unit_in_stack(col_i16 x, col_i16 y);
col_i16 plow_or_clear_tile(void);
col_i16 prompt_dump_cargo(void);
col_i16 refresh_arrived_unit_1008_006f(void);
col_i16 sub_1008_0051(void);
col_i16 sub_1008_cf0f(void *a, char *name);

extern col_i16 colony_at(col_i16 x, col_i16 y);
extern col_i16 colony_window_open(void *w, col_i16 c);

/* same object: 4-byte `push cs; call ` (rule 47) */

















void orders_key_command(ArrivedWin *o, col_i16 key)
{
    register col_i16 c;

    sub_1008_cf0f(&(*g_info_win), "Info Window");
    if (key > 0 && key < 9) {
        arrived_landfall(o, key - 1);
        return;
    }
    if (g_message_pending != 0 && g_message_flag != 0) {
        switch (key) {
        case 0xd1:
            if (g_message_pending != 0 && g_message_flag != 0)
                g_message_flag = 0;
            else {
                o->f4 = 0;
                refresh_arrived_unit_1008_006f();
            }
            break;
        case 0xd0:
            if (g_message_pending != 0 && g_message_flag != 0)
                g_message_flag = 0;
            else {
                c = colony_at(g_cursor_x, g_cursor_y);
                if (c >= 0)
                    if (g_colonies[c].nation == g_view_nation || g_reveal_all != 0)
                        colony_window_open(&(*g_colony_window), c);
            }
            break;
        }
    } else {
        switch (key) {
        case 0xd1:
            if (g_message_pending != 0 && g_message_flag != 0)
                g_message_flag = 0;
            else {
                o->f4 = 0;
                refresh_arrived_unit_1008_006f();
            }
            break;
        case 0xd0:
            if (g_message_pending != 0 && g_message_flag != 0)
                g_message_flag = 0;
            else {
                c = colony_at(g_cursor_x, g_cursor_y);
                if (c >= 0)
                    if (g_colonies[c].nation == g_view_nation || g_reveal_all != 0)
                        colony_window_open(&(*g_colony_window), c);
            }
            break;
        case 'A':
            if (g_order_flags[0] != 0) {
                o->f4 = 0;
                pick_unit_in_stack(g_cursor_x, g_cursor_y);
            }
            break;
        case 'G':
            if (g_order_flags[0xb] != 0) {
                o->f4 = 0;
                orders_port();
            }
            break;
        case 'S':
            if (g_order_flags[4] != 0) {
                o->f4 = 0;
                arrived_unit_orders_1();
            }
            break;
        case 'F':
            if (g_order_flags[2] != 0) {
                o->f4 = 0;
                fortify_unit();
            }
            break;
        case 'R':
            if (g_order_flags[7] != 0) {
                o->f4 = 0;
                build_road();
            }
            break;
        case 'P':
            if (g_order_flags[6] != 0) {
                o->f4 = 0;
                plow_or_clear_tile();
            }
            break;
        case 'B':
            if (g_order_flags[5] != 0) {
                o->f4 = 0;
                build_colony();
            }
            break;
        case 'L':
            if (g_order_flags[8] != 0) {
                o->f4 = 0;
                open_colony_under_unit_1008_0d7f();
            }
            break;
        case 'U':
            if (g_order_flags[9] != 0) {
                o->f4 = 0;
                open_colony_under_unit_1008_0e05();
            }
            break;
        case 'W':
            if (g_order_flags[1] != 0) {
                o->f4 = 0;
                sub_1008_0051();
            }
            break;
        case 'O':
            if (g_order_flags[0xf] != 0) {
                o->f4 = 0;
                prompt_dump_cargo();
            }
            break;
        case 0x144:
            if (g_order_flags[0x10] != 0) {
                o->f4 = 0;
                disband_unit();
            }
            break;
        case 0xa8:
        case 0xc0:
            arrived_landfall(o, 0);
            break;
        case 0xa4:
        case 0xc2:
            arrived_landfall(o, 6);
            break;
        case 0xa6:
        case 0xc3:
            arrived_landfall(o, 2);
            break;
        case 0xa2:
        case 0xc1:
            arrived_landfall(o, 4);
            break;
        case 0xa7:
        case 0xc4:
            arrived_landfall(o, 7);
            break;
        case 0xa9:
        case 0xc5:
            arrived_landfall(o, 1);
            break;
        case 0xa1:
        case 0xc7:
            arrived_landfall(o, 5);
            break;
        case 0xa3:
        case 0xc6:
            arrived_landfall(o, 3);
            break;
        }
    }
    peek_message_only_1008_cefc();
}
