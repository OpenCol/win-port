/* building_output -- shipped at 1018:6a20.
 *
 * Ported by tools/port.py from matched/game/building_output.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern Rec12 far g_rec12_d42b[];                     SEG20:0xd42b
 *
 * The file's own Colony, NationRec52, Nation, Rec12 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 building_slot_of(col_i16 b);
col_i16 colonist_job(col_i16 c);
col_i16 colonist_specialty_get(col_i16 c);
col_i16 colony_building_top(col_i16 b);
col_i16 colony_sol_percent(void);
col_i16 current_colony_building_count(col_i16 b);
col_i16 current_colony_flag_get(col_i16 b);
col_i16 max_int_1018_b34f(col_i16 a, col_i16 b);

#include "colflag.h"
#include "nations.h"
#include "fathers.h"
#include "control.h"
#include "building.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
/* 316 bytes at SEG20:0xc2f4 */
/* 12 bytes at SEG20:0xd42b */





extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);

/* same object: 4-byte `push cs; call ` (rule 47) */









col_i16 building_output(col_i16 c, col_i16 *good)
{
    col_i16 job;
    col_i16 spec;
    col_i16 expert;
    col_i16 out;
    col_i16 slot;
    col_i16 top;
    col_i16 n;
    col_i16 tories;
    col_i16 sol;
    col_i16 div;
    col_i16 pen;
    col_i16 halve;
    register col_i16 base;
    register col_i16 rate;

    halve = 0;
    job = colonist_job(c);
    spec = colonist_specialty_get(c);
    expert = spec == job ? 1 : 0;
    sol = colony_sol_percent();
    tories = (g_cur_colony_rec->pop * (100 - sol) + 50) / 100;
    div = g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_colony_rec->nation].control == CONTROL_HUMAN
        ? 10 - g_difficulty : 10;
    if (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation52[g_cur_colony_rec->nation].control != CONTROL_HUMAN)
        tories = 0;
    pen = -(tories / div);
    if (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY)
        pen++;
    if (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS)
        pen++;
    base = expert ? 3 : 1;
    out = -1;
    slot = building_slot_of(job);
    top = colony_building_top(slot);
    if ((col_i16)g_rec12_d42b[top].level > 0 && (g_nations[g_cur_colony_rec->nation].flags & 0x10))
        halve = 1;
    switch (spec) {
    case 0x1a:
    case 0x1b:
        rate = 1;
        break;
    case 0x19:
        rate = 2;
        break;
    default:
        rate = 3;
        break;
    }
    switch (job) {
    case 13:
        out = 0x10;
        base = expert ? 6 : rate;
        base += pen;
        if (current_colony_flag_get(BUILDING_LUMBER_MILL))
            base = base * 2;
        break;
    case 16:
        base = expert ? 6 : rate;
        out = 0x11;
        base += pen;
        if (current_colony_flag_get(BUILDING_CATHEDRAL))
            base = base * 2;
        if (nation_flag_test(g_cur_colony_rec->nation, FATHER_WILLIAM_PENN))
            base += base >> 1;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 14:
        out = job;
        base = rate;
        base += pen;
        n = current_colony_building_count(slot);
        if (n > 1)
            base += rate;
        if (n > 2)
            base += base >> 1;
        if (expert)
            base = base * 2;
        break;
    case 15:
        out = job;
        base = rate;
        base += pen;
        n = current_colony_building_count(slot);
        if (n > 1)
            base += rate;
        if (n > 2)
            base += base >> 1;
        if (expert)
            base = base * 2;
        break;
    case 17:
        base = rate;
        base += pen;
        out = 0x12;
        if (expert)
            base = base * 2;
        break;
    }
    if (halve)
        base = base >> 1;
    if (good != 0)
        *good = out;
    base = max_int_1018_b34f(0, base);
    return base;
}
