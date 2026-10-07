/* tory_unrest: candidate for 1038:5086 (1005 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE TORY UPRISING. When the royal expedition has nothing left to land,
 * `revolution_check` calls this on the mother country's turn. Loyalists rise 
 * one rebel colony, as royal soldiers.
 *
 * It happens at all only if a roll of 0..difficulty+1 is not 0. Then every colony of `nation` not
 * already marked (bit 0 of +0x1c) is scored:
 *   - twice its Tory percentage of its population, plus difficulty + 1;
 *   - less the attack of every unit standing in the colony;
 *   - -1 if a royal unit is already on a neighbouring land square, or if no neighbouring land
 *     square is free (not ocean, not owned, empty).
 * The best positive score wins and the colony is marked.
 *
 * THE LANDING walks the eight neighbours again and again. On each free square, or one whose
 * owner bit belongs to the mother country, it creates a UNIT_SOLDIERS for the mother country. The
 * soldier is made a veteran (JOB_SOLDIER) on odd counts and a UNIT_DRAGOONS on multiples of three,
 * each only if its own difficulty roll is not 0. Every attempt, successful or not, uses up one
 * point of the count. A pass that lands nobody ends it. If nobody landed at all the mark is cleared;
 * otherwise the colony square is redrawn and TORYUPRISING names the colony.
 *
 * THE COUNT IS THE WRONG COLONY'S. The landing counts down `score`, and after the scan that holds
 * the LAST colony scored, not the best one (`best` is only compared). So the uprising's size
 * depends on whichever colony happened to come last in the scan. That is almost certainly a bug in
 * the shipped game, and the bytes leave no other reading: the loop tests [bp-6], the scan's score slot.
 *
 * `landed = any = 1` stores `any` first; written the other way round, the two stores swap.
 *
 * RENAMED 2026-09-26 from tory_uprising: the Mac build's tory_unrest(short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller revolution_check; callee colony_rebel; callee is_city; callee is_unit; callee next_unit), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "jobs.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);
extern int colony_rebel(void);
extern int terrain_is_ocean(int x, int y);
extern int is_city(int x, int y);
extern int is_unit(int x, int y);
extern void force_map(int x, int y);
extern int popm(char *msg, int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

void tory_unrest(int nation)
{
    int colony;
    int score;
    int tories;
    int best;
    int best_colony;
    int nx;
    int ny;
    int open;
    int landed;
    int any;
    register int u;
    register int dir;

    best = 0;
    best_colony = -1;
    if (rand_range(0, g_game.difficulty + 1) == 0)
        return;
    for (colony = 0; g_game.colony_count > colony; colony++) {
        set_colony(colony);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        if (g_cur_colony_rec->status & 1)
            continue;
        tories = colony_rebel();
        tories = (100 - tories) * 2;
        score = g_cur_colony_rec->pop * tories / 100;
        score += g_game.difficulty + 1;
        for (u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0; u = next_unit(u))
            score -= g_unit_types[g_units[u].type].attack;
        open = 0;
        for (dir = 0; dir < 8; dir++) {
            nx = g_cur_colony_rec->x + g_dir_dx[dir];
            ny = g_cur_colony_rec->y + g_dir_dy[dir];
            if (terrain_is_ocean(nx, ny))
                continue;
            if (is_city(nx, ny) >= 0)
                continue;
            u = top_unit_in_square(nx, ny);
            if (u < 0) {
                open++;
                continue;
            }
            if (NATION(g_units[u]) == (unsigned char)g_game.mother_country_idx)
                score = -1;
        }
        if (open == 0)
            score = -1;
        if (score > best) {
            best = score;
            best_colony = colony;
        }
    }
    if (best_colony < 0)
        goto out;
    set_colony(best_colony);
    g_cur_colony_rec->status |= 1;
    any = 0;
    while (score > 0) {
        landed = 0;
        for (dir = 0; score > 0 && dir < 8; dir++) {
            nx = g_cur_colony_rec->x + g_dir_dx[dir];
            ny = g_cur_colony_rec->y + g_dir_dy[dir];
            if (terrain_is_ocean(nx, ny))
                continue;
            if (is_city(nx, ny) >= 0)
                continue;
            u = is_unit(nx, ny);
            if (u >= 0 && (signed char)g_game.mother_country_idx != u)
                continue;
            u = create_unit(UNIT_SOLDIERS, g_game.mother_country_idx, nx, ny);
            if (u >= 0) {
                landed = any = 1;
                if ((score & 1) && rand_range(0, g_game.difficulty + 1))
                    g_units[u].spec = JOB_SOLDIER;
                if (score % 3 == 0 && rand_range(0, g_game.difficulty + 1))
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
        force_map(g_cur_colony_rec->x, g_cur_colony_rec->y);
        set_name_slot_1038_8775(0, g_cur_colony_rec->name);
        popm("TORYUPRISING", 1);
    }
out:
    ;
}
