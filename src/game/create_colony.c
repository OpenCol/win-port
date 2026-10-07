/* create_colony -- shipped at 1018:af9d.
 *
 * Ported by tools/port.py from matched/game/create_colony.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern unsigned char far g_nation_colony_count[4];   DGROUP:0x4b48
 *   extern int g_cur_colony_idx;                         DGROUP:0x4ba2
 *   extern int g_ocean_x;                                DGROUP:0x4bc8
 *   extern int g_ocean_y;                                DGROUP:0x4bca
 *
 * The file's own Colony, NationRec52 dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: f1c -> status, f1f -> pop, f8d -> export_goods, f90 -> changed, f92 -> hammers, f94 -> item, f95 -> warehouses, f96 -> fort, f97 -> mine, fc2 -> bells, fc6 -> bells_cap
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_colonist_index(col_i16 u);
void colonist_specialty_set(col_i16 a, col_i16 b);
void colony_flag2_set(col_i16 n, col_i16 v);
void colony_flag_set(col_i16 n, col_i16 v);
void colony_native_claims(void);
void colony_refresh_screen(void);
void colony_set_job(col_i16 a, col_i16 b);
void set_current_colony(col_i16 c);
void show_text_dialog_1018_b3d1(char *name);

#include "fathers.h"
#include "building.h"
#include "nations.h"
#include "control.h"
#include "jobs.h"
/* +1 is CONTROL: 0 human, 1 computer, 2 defeated */






extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char * map_plane1_ptr(col_i16 x, col_i16 y);
extern unsigned char map_plane3_at(col_i16 x, col_i16 y);
extern unsigned char square_region(col_i16 x, col_i16 y);
extern col_i16 find_adjacent_ocean(col_i16 x, col_i16 y);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
extern void reveal_around_colony(col_i16 c, col_i16 nation);
extern void *memset_rtl(void *p, col_i16 v, col_u16 n);











col_i16 create_colony(col_i16 nation, col_i16 x, col_i16 y, col_i16 unit)
{
    col_i16 colonist;
    col_i16 i;
    register col_i16 col;
    register col_i16 u;

    u = unit;
    col = -1;
    if (g_colony_count >= 0x30) {
        if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
            show_text_dialog_1018_b3d1("TOOMANYCOLONIES");
        goto done;
    }
    if (on_map(x, y) == 0)
        goto done;
    col = g_colony_count;
    g_colony_count++;
    g_nation_colony_count[nation]++;
    set_current_colony(col);
    *map_plane1_ptr(x, y) |= 2;
    g_cur_colony_rec->nation = (unsigned char)nation;
    g_cur_colony_rec->x = (unsigned char)x;
    g_cur_colony_rec->y = (unsigned char)y;
    g_cur_colony_rec->f1b = 0;
    g_cur_colony_rec->status = 0;
    g_cur_colony_rec->f1d = 0;
    g_cur_colony_rec->pop = 0;
    g_cur_colony_rec->hammers = 0;
    g_cur_colony_rec->warehouses = 0;
    g_cur_colony_rec->fort = 0;
    g_cur_colony_rec->f98 = 0;
    g_cur_colony_rec->mine = 0;
    g_cur_colony_rec->changed = 0;
    g_cur_colony_rec->f8c = 0;
    g_cur_colony_rec->export_goods = 0xff;
    g_cur_colony_rec->f8e = 0;
    g_cur_colony_rec->f8f = 0;
    g_cur_colony_rec->bells_cap = 100;
    g_cur_colony_rec->bells = 0;
    memset_rtl(((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x8a)), 0, 2);
    colony_flag2_set(1, 1);
    colony_flag2_set(2, 1);
    colony_flag2_set(3, 1);
    colony_flag2_set(4, 1);
    colony_flag2_set(6, 1);
    colony_flag2_set(7, 1);
    colony_flag2_set(9, 1);
    colony_flag2_set(0xa, 1);
    colony_flag2_set(0xb, 1);
    colony_flag2_set(0xc, 1);
    memset_rtl(((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x84)), 0, 6);
    colony_flag_set(BUILDING_CARPENTERS_SHOP, 1);
    colony_flag_set(BUILDING_TOWN_HALL, 1);
    colony_refresh_screen();
    memset_rtl(((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x9a)), 0, 0x20);
    memset_rtl(((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x70)), -1, 0x14);
    if (u >= 0) {
        colonist = unit_colonist_index(u);
        if (colonist >= 0) {
            colony_set_job(colonist, 0);
        } else {
            g_colonies[col].pop = 1;
            colony_set_job(0, 0);
            colonist_specialty_set(0, JOB_COLONIST_ALT);
        }
    }
    colony_native_claims();
    colony_refresh_screen();
    for (i = 0; i < 4; i++) {
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0xba))[i] = 1;
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0xbe))[i] = 0;
        if (i >= 4 || (map_plane3_at(x, y) & (0x10 << i)))
            ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0xba))[i] = 1;
    }
    if (find_adjacent_ocean(x, y) && square_region(g_ocean_x, g_ocean_y) == 1)
        g_colonies[col].status |= 0x40;
    g_cur_colony_rec->item = 0xf;
    if (g_cur_colony_rec->status & 0x40)
        g_cur_colony_rec->item = 6;
    for (i = 0; i < 4; i++) {
        if (nation_flag_test(i, FATHER_FRANCISCO_DE_CORONADO))
            reveal_around_colony(g_cur_colony_idx, i);
    }
done:
    return col;
}
