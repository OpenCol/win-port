/* building_table_init -- shipped at 1008:81d4.
 *
 * Ported by tools/port.py from matched/game/building_table_init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_map_pixels;                             DGROUP:0x4b2e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 building_table_set_fields(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 i);
col_i16 recruit_set(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 i);

/* same object: 4-byte `push cs; call ` */



void building_table_init(void)
{
    register col_i16 i;
    register col_i16 j;

    i = -1;
    g_map_pixels = 0;
    building_table_set_fields(0, -1, -1, 1, ++i);
    building_table_set_fields(1, 0, -1, 2, i);
    building_table_set_fields(2, 1, -1, -1, i);
    building_table_set_fields(3, -1, -1, 4, ++i);
    building_table_set_fields(4, 3, -1, 5, i);
    building_table_set_fields(5, 4, -1, -1, i);
    building_table_set_fields(6, -1, -1, 7, ++i);
    building_table_set_fields(7, 6, -1, 8, i);
    building_table_set_fields(8, 7, -1, -1, i);
    building_table_set_fields(9, -1, -1, -1, ++i);
    building_table_set_fields(10, 9, -1, 11, i);
    building_table_set_fields(11, 10, -1, -1, i);
    building_table_set_fields(30, 11, -1, 31, i);
    building_table_set_fields(31, 30, -1, -1, i);
    building_table_set_fields(12, -1, -1, 13, ++i);
    building_table_set_fields(13, 12, -1, 14, i);
    building_table_set_fields(14, 13, -1, -1, i);
    building_table_set_fields(15, -1, -1, 16, ++i);
    building_table_set_fields(16, 15, -1, -1, i);
    building_table_set_fields(17, -1, -1, -1, i);
    building_table_set_fields(18, -1, -1, -1, ++i);
    building_table_set_fields(19, -1, -1, 20, ++i);
    building_table_set_fields(20, 19, -1, -1, i);
    building_table_set_fields(21, -1, -1, 22, ++i);
    building_table_set_fields(22, 21, -1, 23, i);
    building_table_set_fields(23, 22, -1, -1, i);
    building_table_set_fields(24, -1, -1, 25, ++i);
    building_table_set_fields(25, 24, -1, 26, i);
    building_table_set_fields(26, 25, -1, -1, i);
    building_table_set_fields(27, -1, -1, 28, ++i);
    building_table_set_fields(28, 27, -1, 29, i);
    building_table_set_fields(29, 28, -1, -1, i);
    building_table_set_fields(32, -1, -1, 33, ++i);
    building_table_set_fields(33, 32, -1, 34, i);
    building_table_set_fields(34, 33, -1, -1, i);
    building_table_set_fields(35, -1, -1, 36, ++i);
    building_table_set_fields(36, 35, -1, -1, i);
    building_table_set_fields(37, -1, -1, 38, ++i);
    building_table_set_fields(38, 37, -1, -1, i);
    building_table_set_fields(39, -1, -1, 40, ++i);
    building_table_set_fields(40, 39, -1, 41, i);
    building_table_set_fields(41, 40, -1, -1, i);
    j = -1;
    recruit_set(0, 11, 500, 2, -1, ++j);
    recruit_set(0, 13, 1000, 2, -1, ++j);
    recruit_set(0, 14, 2000, 4, -1, ++j);
    recruit_set(0, 15, 3000, 6, -1, ++j);
    recruit_set(0, 16, 2000, 3, -1, ++j);
    recruit_set(0, 17, 5000, 8, -1, ++j);
}
