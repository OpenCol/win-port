/* create_unit -- shipped at 1048:bbe2.
 *
 * Ported by tools/port.py from matched/game/create_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_text_value;                         DGROUP:0x251a
 *   extern int far g_cur_tribe_nation;                   DGROUP:0x4c32
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ask_text_resource(char *name);
col_i16 show_text_dialog_1048_e3ae(char *name, col_i16 a);

#include "jobs.h"
#include "units.h"
#include "gameflag.h"
extern col_i16 create_unit_1038_2246(col_i16 type, col_i16 nation, col_i16 x, col_i16 y);
extern col_i16 find_nearest_colony(col_i16 x, col_i16 y, col_i16 nation, col_i16 land);
extern col_i16 find_nearest_village(col_i16 x, col_i16 y, col_i16 a, col_i16 owner);
extern void redraw_map_1040_11b7(col_i16 a);

/* same object: 4-byte `push cs; call ` (rule 47) */



void create_unit(void)
{
    col_i16 choice;
    col_i16 village;
    col_i16 type;
    register col_i16 nation;
    register col_i16 u;

    if (g_unit_count < 300) {
        nation = g_turn_nation;
        u = -1;
        do {
            g_text_value = 1;
            if (g_game_flags & GAMEFLAG_INDEPENDENCE)
                choice = ask_text_resource("CREATE2");
            else
                choice = ask_text_resource("CREATE");
            switch (choice) {
            case 1:
                u = create_unit_1038_2246(UNIT_COLONISTS, nation, g_cursor_x, g_cursor_y);
                break;
            case 2:
                u = create_unit_1038_2246(UNIT_PIONEERS, nation, g_cursor_x, g_cursor_y);
                break;
            case 3:
                u = create_unit_1038_2246(UNIT_SOLDIERS, nation, g_cursor_x, g_cursor_y);
                break;
            case 4:
                u = create_unit_1038_2246(UNIT_MISSIONARIES, nation, g_cursor_x, g_cursor_y);
                break;
            case 5:
                u = create_unit_1038_2246(UNIT_SCOUTS, nation, g_cursor_x, g_cursor_y);
                break;
            case 6:
                u = create_unit_1038_2246(UNIT_ARTILLERY, nation, g_cursor_x, g_cursor_y);
                break;
            case 7:
                u = create_unit_1038_2246(UNIT_WAGON_TRAIN, nation, g_cursor_x, g_cursor_y);
                g_units[u].home = find_nearest_colony(g_cursor_x, g_cursor_y, nation, -1);
                break;
            case 8:
                u = create_unit_1038_2246(UNIT_TREASURE_TRAIN, nation, g_cursor_x, g_cursor_y);
                g_units[u].spec = JOB_SUGAR_PLANTER;
                break;
            case 9:
                g_text_value = 0;
                choice = show_text_dialog_1048_e3ae("CSHIP", 0);
                if (choice > 0)
                    u = create_unit_1038_2246(choice + 0xc, nation, g_cursor_x, g_cursor_y);
                break;
            case 10:
            case 11:
            case 12:
            case 13:
                if ((g_game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                    village = find_nearest_village(g_cursor_x, g_cursor_y, -1, -1);
                    if (village >= 0)
                        u = create_unit_1038_2246(choice + 9, g_cur_tribe_nation,
                                                  g_cursor_x, g_cursor_y);
                } else {
                    nation = choice < 0xc ? g_player_nation : g_mother_country_idx;
                    if (choice == 10)
                        type = 9;
                    if (choice == 11)
                        type = 7;
                    if (choice == 12)
                        type = 6;
                    if (choice == 13)
                        type = 8;
                    u = create_unit_1038_2246(type, nation, g_cursor_x, g_cursor_y);
                }
                break;
            case 14:
                g_text_value = 2;
                if ((g_game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                    nation = ask_text_resource("FOREIGN") - 1;
                    if (nation < 0)
                        nation = g_turn_nation;
                } else {
                    nation = ask_text_resource("FOREIGN2") - 1;
                    switch (nation) {
                    case 1:
                        nation = g_mother_country_idx;
                        break;
                    case 0:
                    default:
                        nation = g_player_nation;
                        break;
                    }
                }
                break;
            }
        } while (choice == 14);
        if (u >= 0)
            g_units[u].flags |= 0xf0;
        redraw_map_1040_11b7(1);
    }
}
