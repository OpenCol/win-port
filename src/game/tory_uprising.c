/* tory_uprising -- shipped at 1038:5086.
 *
 * Ported by tools/port.py from matched/game/tory_uprising.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 create_unit_1038_2246(col_i16 type, col_i16 nation, col_i16 x, col_i16 y);
col_i16 unit_at(col_i16 x, col_i16 y);
col_i16 unit_chain_next(col_i16 u);
void set_name_slot_1038_8775(col_i16 slot, char *name);

#include "units.h"
#include "jobs.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void set_current_colony(col_i16 c);
extern col_i16 colony_sol_percent(void);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 square_nation(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);
extern void redraw_tile(col_i16 x, col_i16 y);
extern col_i16 text_message_show(char *msg, col_i16 n);
/* same object: 4-byte `push cs; call ` (rule 47) */





void tory_uprising(col_i16 nation)
{
    col_i16 c;
    col_i16 score;
    col_i16 tories;
    col_i16 best;
    col_i16 best_colony;
    col_i16 nx;
    col_i16 ny;
    col_i16 open;
    col_i16 landed;
    col_i16 any;
    register col_i16 u;
    register col_i16 dir;

    best = 0;
    best_colony = -1;
    if (rand_range(0, g_difficulty + 1) == 0)
        return;
    for (c = 0; g_colony_count > c; c++) {
        set_current_colony(c);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        if (g_cur_colony_rec->status & 1)
            continue;
        tories = colony_sol_percent();
        tories = (100 - tories) * 2;
        score = g_cur_colony_rec->pop * tories / 100;
        score += g_difficulty + 1;
        for (u = unit_at(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0; u = unit_chain_next(u))
            score -= g_unit_types[g_units[u].type].attack;
        open = 0;
        for (dir = 0; dir < 8; dir++) {
            nx = g_cur_colony_rec->x + g_dir_dx[dir];
            ny = g_cur_colony_rec->y + g_dir_dy[dir];
            if (ocean(nx, ny))
                continue;
            if (square_nation(nx, ny) >= 0)
                continue;
            u = unit_at(nx, ny);
            if (u < 0) {
                open++;
                continue;
            }
            if (NATION(g_units[u]) == (unsigned char)g_mother_country_idx)
                score = -1;
        }
        if (open == 0)
            score = -1;
        if (score > best) {
            best = score;
            best_colony = c;
        }
    }
    if (best_colony < 0)
        goto out;
    set_current_colony(best_colony);
    g_cur_colony_rec->status |= 1;
    any = 0;
    while (score > 0) {
        landed = 0;
        for (dir = 0; score > 0 && dir < 8; dir++) {
            nx = g_cur_colony_rec->x + g_dir_dx[dir];
            ny = g_cur_colony_rec->y + g_dir_dy[dir];
            if (ocean(nx, ny))
                continue;
            if (square_nation(nx, ny) >= 0)
                continue;
            u = square_nation_bit0(nx, ny);
            if (u >= 0 && (signed char)g_mother_country_idx != u)
                continue;
            u = create_unit_1038_2246(UNIT_SOLDIERS, g_mother_country_idx, nx, ny);
            if (u >= 0) {
                landed = any = 1;
                if ((score & 1) && rand_range(0, g_difficulty + 1))
                    g_units[u].spec = JOB_SOLDIER;
                if (score % 3 == 0 && rand_range(0, g_difficulty + 1))
                    g_units[u].type = UNIT_DRAGOONS;
            }
            score--;
        }
        if (landed == 0)
            score = 0;
    }
    if (any == 0) {
        g_cur_colony_rec->status &= ~1;
    } else {
        redraw_tile(g_cur_colony_rec->x, g_cur_colony_rec->y);
        set_name_slot_1038_8775(0, ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
        text_message_show("TORYUPRISING", 1);
    }
out:
    ;
}
