/* colony_site_layout -- shipped at 1018:5b07.
 *
 * Ported by tools/port.py from matched/game/colony_site_layout.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern BuildingRec far g_buildings[];                SEG20:0xd422
 *   extern unsigned char far g_site_kind[];              SEG21:0x0000
 *   extern signed char far g_tbl21_1eb[];                SEG21:0x01eb
 *   extern signed char far g_site_building[];            SEG21:0x01dc
 *   extern unsigned char g_kind_count[];                 DGROUP:0x0bee
 *   extern unsigned char g_kind_base[];                  DGROUP:0x0bf3
 *
 * The file's own BuildingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 current_colony_flag_get(col_i16 bit);
void colony_seed_random(void);

/* 12 bytes at SEG20:0xd422 */






extern col_i16 rand_range(col_i16 lo, col_i16 hi);

/* same object: 4-byte `push cs; call ` */



void colony_site_layout(void)
{
    col_i16 r;
    col_i16 i;
    col_i16 base;
    col_i16 count;
    col_i16 cls;
    col_i16 kind;
    col_i16 used[5];
    col_i16 cls_slot[15];
    register col_i16 j;
    register col_i16 t;

    colony_seed_random();
    for (i = 0; i < 15; i++) {
        g_tbl21_1eb[i] = -1;
        g_site_building[i] = -1;
        cls_slot[i] = -1;
    }
    for (i = 0; i < 5; i++)
        used[i] = 0;
    for (i = 0; i < 5; i++)
        for (j = 0; g_kind_count[i] > j; j++)
            g_site_kind[g_kind_base[i] + j] = i;
    for (i = 0; i < 15; i++) {
        t = g_site_kind[i];
        base = g_kind_base[t];
        count = g_kind_count[t];
        do {
            r = rand_range(0, count - 1) + base;
        } while ((col_i16)g_tbl21_1eb[r] >= 0);
        g_tbl21_1eb[r] = i;
    }
    for (i = 0; i < 0x2a; i++) {
        cls = g_buildings[i].family;
        if (cls_slot[cls] < 0) {
            kind = g_buildings[i].kind;
            t = used[kind]++;
            t += g_kind_base[kind];
            cls_slot[cls] = t;
        }
    }
    for (i = 0; i < 0x2a; i++) {
        if (current_colony_flag_get(i) != 0 || i == 0) {
            cls = g_buildings[i].family;
            t = cls_slot[cls];
            g_site_building[g_tbl21_1eb[t]] = i;
        }
    }
}
