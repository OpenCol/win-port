/* royal_expedition_lands -- shipped at 1038:5473.
 *
 * Ported by tools/port.py from matched/game/royal_expedition_lands.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_royal_forces[];                     SEG20:0x77dc
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern unsigned char far g_unit_tally[][0x13];       SEG20:0xd9de
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern char far g_map_window;                        SEG20:0x0658
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colony_defence_str(void);
col_i16 create_unit_1038_2246(col_i16 kind, col_i16 nation, col_i16 x, col_i16 y);
col_i16 max_int_1038_86a6(col_i16 a, col_i16 b);
col_i16 min_int_1038_8680(col_i16 a, col_i16 b);
col_i16 select_state_code(col_i16 kind, col_i16 nation);
col_i16 tile_unit_tally(col_i16 u, col_i16 kind);
col_i16 unit_at(col_i16 x, col_i16 y);
col_i16 unit_chain_next(col_i16 u);
void seize_goods(col_i16 x, col_i16 y);
void set_name_slot_1038_8775(col_i16 slot, char *name);
void unit_chain_relink(col_i16 u, col_i16 x, col_i16 y);
void unit_nation_flag_set(col_i16 u, col_i16 n);
void unit_refresh_move_points(col_i16 u);

#include "royal.h"
#include "units.h"
#define ABSDIFF(a, b) ((a) - (b) > 0 ? (a) - (b) : ~((a) - (b)) + 1)

extern void set_current_colony(col_i16 c);
extern col_i16 colony_sol_percent(void);
extern void sort_int_pairs_by_key(col_i16 n, col_i16 *ids, col_i16 *keys);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 square_nation(col_i16 x, col_i16 y);
extern unsigned char square_region(col_i16 x, col_i16 y);
extern col_i16 square_region_if_land(col_i16 x, col_i16 y);
extern unsigned char map_plane2_high_nibble(col_i16 x, col_i16 y);
extern void map_goto(col_i16 x, col_i16 y, col_i16 flag);
extern void MapWin_SlideIcon(char *w, col_i16 u, col_i16 speed, col_i16 a, col_i16 x0, col_i16 y0,
                                 col_i16 x1, col_i16 y1);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void music_set_mood(col_i16 a);
extern void redraw_map_area(col_i16 x, col_i16 y, col_i16 a, col_i16 b, col_i16 c);

/* 5-byte NOP form, so extern (rule 47) */
extern col_i16 * max_ptr_1038_8d92(col_i16 *a, col_i16 *b);

/* same object: 4-byte `push cs; call ` (rule 47) */














void royal_expedition_lands(col_i16 nation)
{
    register col_i16 unit;
    register col_i16 nx;
    col_i16 colony;
    col_i16 k;
    col_i16 n;
    col_i16 d;
    col_i16 d2;
    col_i16 region;
    col_i16 region2;
    col_i16 ny;
    col_i16 nx2;
    col_i16 ny2;
    col_i16 lx;
    col_i16 ly;
    col_i16 cx;
    col_i16 cy;
    col_i16 score;
    col_i16 best;
    col_i16 troops;
    col_i16 strength;
    col_i16 cnt3;
    col_i16 cnt1;
    col_i16 cap;
    col_i16 kind;
    col_i16 type;
    col_i16 placed;
    col_i16 found;
    col_i16 second;
    col_i16 least;
    col_i16 tally;
    col_i16 contested;
    col_i16 skip;
    col_i16 pass;
    col_i16 changed;
    col_i16 b;
    col_i16 a;
    col_i16 wipe;
    col_i16 total;
    col_i16 eighth;
    col_i16 one;
    col_i16 eighth2;
    col_i16 one2;
    col_i16 ids[10];
    col_i16 keys[10];
    col_i16 defence[10];

    wipe = 0;
    if (g_royal_forces[ROYAL_MAN_O_WAR] != 0) {
        total = g_royal_forces[ROYAL_REGULARS] + g_royal_forces[ROYAL_CAVALRY] + g_royal_forces[ROYAL_MAN_O_WAR] + g_royal_forces[ROYAL_ARTILLERY];
        if (total <= 4 || total - g_royal_forces[ROYAL_MAN_O_WAR] == 0)
            wipe = 1;
        if (g_royal_forces[ROYAL_MAN_O_WAR] == total)
            goto done;
    } else {
        if (g_unit_tally[g_mother_country_idx][UNIT_MAN_O_WAR] == 0)
            g_royal_forces[ROYAL_MAN_O_WAR]++;
        goto done;
    }
    n = 0;
    for (colony = 0; colony < g_colony_count; colony++) {
        set_current_colony(colony);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        if (!(g_cur_colony_rec->status & 0x40))
            continue;
        if (n < 10) {
            b = a = 100 - colony_sol_percent();
            b = (b + 25) * (col_i16)g_cur_colony_rec->pop;
            unit = unit_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
            b -= tile_unit_tally(unit, 10) * 0x4b;
            b = max_int_1038_86a6(a, b);
            keys[n] = b;
            ids[n] = (unsigned char)colony;
            n++;
        }
    }
    if (n != 0)
        sort_int_pairs_by_key(n, ids, keys);
    for (k = 0; k < n; k++) {
        colony = ids[k];
        set_current_colony(colony);
        defence[k] = colony_defence_str();
        for (d = 0; d < 8; d++) {
            cx = g_cur_colony_rec->x + g_dir_dx[d];
            cy = g_cur_colony_rec->y + g_dir_dy[d];
            if (ocean(cx, cy))
                continue;
            if ((unsigned char)(g_units[unit].flags & UNIT_NATION_MASK)
                != (unsigned char)g_mother_country_idx)
                continue;
            unit = unit_at(cx, cy);
            while (unit >= 0 && defence[k] > 0) {
                if (g_unit_types[g_units[unit].type].attack != 0)
                    defence[k]--;
                unit = unit_chain_next(unit);
            }
        }
    }
    changed = 1;
    for (pass = 0; changed != 0 && pass < 3; pass++) {
        skip = 0;
        while (skip < n && changed != 0) {
            changed = 0;
            colony = ids[n - 1 - skip];
            set_current_colony(colony);
            strength = defence[n - 1 - skip];
            troops = strength;
            troops = max_int_1038_86a6(1, troops);
            cap = *max_ptr_1038_8d92((one = 1, &one), (eighth = troops >> 3, &eighth));
            if (g_royal_forces[ROYAL_REGULARS] >= g_royal_forces[ROYAL_CAVALRY] + g_royal_forces[ROYAL_ARTILLERY])
                cap = 1;
            cnt1 = min_int_1038_8680(g_royal_forces[ROYAL_CAVALRY], cap);
            cnt3 = min_int_1038_8680(g_royal_forces[ROYAL_ARTILLERY], cap);
            if (pass != 0)
                troops = min_int_1038_8680(g_unit_types[0x12].holds, troops);
            if (pass < 2) {
                if (g_royal_forces[ROYAL_REGULARS] + cnt1 + cnt3 < troops) {
                    skip++;
                    changed = 1;
                }
            }
        }
    }
    troops = min_int_1038_8680(g_unit_types[0x12].holds, troops);
    if (skip >= n)
        goto done;
    region = square_region_if_land(g_cur_colony_rec->x, g_cur_colony_rec->y);
    best = 0;
    for (d = 0; d < 8; d++) {
        nx = g_cur_colony_rec->x + g_dir_dx[d];
        ny = g_cur_colony_rec->y + g_dir_dy[d];
        score = 0;
        if (!ocean(nx, ny))
            continue;
        if (square_region(nx, ny) != 1)
            continue;
        for (d2 = 0; d2 < 8; d2++) {
            nx2 = g_dir_dx[d2] + nx;
            ny2 = g_dir_dy[d2] + ny;
            if (ocean(nx2, ny2))
                continue;
            if (square_region_if_land(nx2, ny2) != region)
                continue;
            if (square_nation(nx2, ny2) >= 0)
                continue;
            score++;
        }
        if (score == 0)
            continue;
        unit = unit_at(nx, ny);
        if (unit >= 0 && (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK)
                         != (unsigned char)g_mother_country_idx) {
            do {
                if (g_units[unit].type == UNIT_MAN_O_WAR)
                    score = 1;
                unit = unit_chain_next(unit);
            } while (unit >= 0);
        }
        if (score < 0)
            continue;
        if (score > best) {
            best = score;
            lx = nx;
            ly = ny;
        }
    }
    if (best <= 0)
        goto done;
    seize_goods(lx, ly);
    g_royal_forces[ROYAL_MAN_O_WAR]--;
    unit = create_unit_1038_2246(UNIT_MAN_O_WAR, g_mother_country_idx, lx, ly);
    if (unit < 0)
        goto done;
    unit_nation_flag_set(unit, nation);
    map_goto(lx, ly, 0);
    music_set_mood(3);
    set_name_slot_1038_8775(0, ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
    text_message_show("INVASION", 1);
    wipe = 0;
    cnt3 = 0;
    cnt1 = 0;
    cap = *max_ptr_1038_8d92((one2 = 1, &one2), (eighth2 = strength >> 3, &eighth2));
    if (g_royal_forces[ROYAL_REGULARS] >= 2)
        cap = min_int_1038_8680(2, cap);
    if (g_royal_forces[ROYAL_REGULARS] >= g_royal_forces[ROYAL_CAVALRY] + g_royal_forces[ROYAL_ARTILLERY])
        cap = 1;
    troops = max_int_1038_86a6(3, troops);
    found = 0;
    second = 0;
    least = 0x63;
    while (troops != 0) {
        placed = 0;
        for (d = 0; d < 8; d++) {
            nx = g_dir_dx[d] + lx;
            ny = g_dir_dy[d] + ly;
            if (ocean(nx, ny))
                continue;
            if (square_nation(nx, ny) >= 0)
                continue;
            if (ABSDIFF(nx, g_cur_colony_rec->x) > 1)
                continue;
            if (ABSDIFF(ny, g_cur_colony_rec->y) > 1)
                continue;
            region2 = square_region_if_land(nx, ny);
            if (region != region2)
                continue;
            if (troops > 0) {
                kind = 0;
                contested = 0;
                if (map_plane2_high_nibble(nx, ny) != (unsigned char)g_mother_country_idx) {
                    unit = unit_at(nx, ny);
                    tally = tile_unit_tally(unit, 2);
                    if (second == 0) {
                        if (tally < least) {
                            found = 1;
                            least = tally;
                        }
                        continue;
                    }
                    if (tally > least)
                        continue;
                    contested = 1;
                } else {
                    if (cnt1 < cap && g_royal_forces[ROYAL_CAVALRY] != 0) {
                        cnt1++;
                        kind = 1;
                    } else if (cnt3 < cap && g_royal_forces[ROYAL_ARTILLERY] != 0) {
                        cnt3++;
                        kind = 3;
                    }
                }
                if (g_royal_forces[kind] <= 0)
                    continue;
                type = select_state_code(kind, g_mother_country_idx);
                unit = create_unit_1038_2246(type, g_mother_country_idx, -2, -2);
                if (unit < 0) {
                    troops = 0;
                    break;
                }
                unit_refresh_move_points(unit);
                unit_nation_flag_set(unit, nation);
                MapWin_SlideIcon(&g_map_window, unit, 0xc0, -1, lx, ly, nx, ny);
                if (contested != 0) {
                    seize_goods(nx, ny);
                    unit = unit_at(-2, -2);
                }
                unit_chain_relink(unit, nx, ny);
                redraw_map_area(g_cur_colony_rec->x - 2, g_cur_colony_rec->y - 2,
                                          5, 5, 1);
                g_royal_forces[kind]--;
                troops--;
                placed = 1;
            }
        }
        if (placed == 0) {
            if (found == 0 || second != 0)
                troops = 0;
            else
                second = 1;
        } else {
            second = 0;
            found = 0;
            least = 0x63;
        }
    }
done:
    if (wipe != 0) {
        for (kind = 0; kind < 4; kind++)
            g_royal_forces[kind] = 0;
    }
}
