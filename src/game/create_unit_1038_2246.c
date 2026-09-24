/* create_unit_1038_2246 -- shipped at 1038:2246.
 *
 * Ported by tools/port.py from matched/game/create_unit_1038_2246.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_nation_unit_count[4];     DGROUP:0x4b44
 *   extern int far g_turn_counter;                       SEG20:0x7790
 *
 * The file's own Unit, NationRec52, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void show_text_dialog_1038_865b(char *name);
void unit_place_at(col_i16 u, col_i16 x, col_i16 y);

#include "nations.h"
#include "control.h"
#include "units.h"
#include "jobs.h"
/* 0x1c bytes at SEG20:0xa208 */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

/* 14 bytes at SEG20:0x7ac8 */




extern col_i16 colony_at(col_i16 x, col_i16 y);
extern void explore_square(col_i16 u);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */




col_i16 create_unit_1038_2246(col_i16 type, col_i16 nation, col_i16 x, col_i16 y)
{
    col_i16 colony;
    register col_i16 u;
    register col_i16 ux;

    ux = x;
    u = -1;
    if ((nation >= NATION_EUROPEAN_COUNT || g_nation52[nation].control != CONTROL_HUMAN) && g_unit_count >= 0x124)
        goto done;
    if (g_unit_count >= 0x12c || (nation < NATION_EUROPEAN_COUNT && g_nation_unit_count[nation] > 0xc8)) {
        if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
            show_text_dialog_1038_865b("TOOMANYUNITS");
        goto done;
    }
    u = g_unit_count;
    g_unit_count++;
    g_nation_unit_count[nation] = g_nation_unit_count[nation] + 1;
    g_units[u].type = (unsigned char)type;
    g_units[u].flags = (unsigned char)nation;
    g_units[u].moves = 0;
    g_units[u].mission = 0x58;
    g_units[u].f4 = 0;
    g_units[u].orders = ORDERS_NONE;
    g_units[u].cargo_count = 0;
    ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[6] = 0;
    ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[0] = 0;
    ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[1] = 0;
    ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[2] = 0xff;
    if (nation >= NATION_EUROPEAN_COUNT)
        *(col_i16 *)&((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[2] = g_turn_counter;
    g_units[u].home = 0xff;
    colony = colony_at(ux, y);
    if (colony >= 0)
        g_units[u].home = (unsigned char)colony;
    if (g_unit_types[g_units[u].type].holds > 0) {
        g_units[u].spec = JOB_FARMER;
        if (g_units[u].type == UNIT_WAGON_TRAIN)
            ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[4] = 0;
    } else {
        g_units[u].spec = JOB_COLONIST_ALT;
        if (g_units[u].type == UNIT_CONTINENTAL_CAVALRY || g_units[u].type == UNIT_CONTINENTAL_ARMY)
            g_units[u].spec = JOB_SOLDIER;
    }
    if (g_units[u].type == UNIT_PIONEERS)
        ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[5] = 0x64;
    g_units[u].link = -1;
    g_units[u].link2 = -1;
    g_units[u].x = 0xff;
    g_units[u].y = 0xff;
    unit_place_at(u, ux, y);
    explore_square(u);
done:
    return u;
}
