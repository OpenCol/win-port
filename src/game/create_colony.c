/* create_colony: candidate for 1018:af9d (908 bytes).  Verdict: RELOCATABLE.
 * symbol: create_colony
 *
 * Creates a colony, and it is `create_unit`'s twin in every structural way:
 * one limit with a text resource behind it, then the whole record initialised field by
 * field, then the world told about it. The Macintosh build names it the same way too --
 * `create_colony(short, short, short, short)`, four arguments, matching this exactly, next
 * to the `create_unit(short, short, short, short)` that named the other one.
 *
 * ONE limit, not two: 48 colonies (0x30). Over it, a human-controlled European power is shown
 * TOOMANYCOLONIES -- DGROUP:0xdaf, read out of the shipped image, the same route that
 * settled CheatMenuColonist's ceilings -- and every other nation, computer Europeans included, fails silently. Then
 * the square must be on the map. Either failure returns -1.
 *
 * THIS FUNCTION MAPS THE WHOLE COLONY RECORD. Every field from +0 to +0xc9 is either
 * written here or skipped over by a memset, so the 0xca-byte record's shape falls out of
 * one function: the position and nation at +0/+1/+0x1a, single bytes at +0x1b..+0x1f, a
 * 0x14-byte block at +0x70 filled with -1, a 6-byte block at +0x84, two bytes at +0x8a,
 * four bytes at +0x8c..+0x8f, words at +0x90/+0x92/+0x98, bytes at +0x94..+0x97, a
 * 0x20-byte block at +0x9a, two 4-byte arrays at +0xba and +0xbe, and two LONGS at +0xc2
 * and +0xc6 -- the second set to 100, the only non-zero, non-flag initial value in the
 * record.
 *
 * Ten `custom_set` calls (1, 2, 3, 4, 6, 7, 9, 0xa, 0xb, 0xc -- note 5 and 8 are
 * missing) and two `set_building` calls (0x23 and 9) set the starting flags. The four
 * bytes at +0xba are all set to 1 (the plane-3 test on `0x10 << i` only stores 1 again), an
 * adjacent ocean square in connected region 1 (`continent_at`) sets bit 0x40 of +0x1c, +0x94 starts at 0xf and drops to 6 when that bit is
 * on, and every nation holding bit 6 gets the colony revealed to it.
 *
 * TWO CONTRASTS WITH CheatMenuColonist, both in the bytes rather than the behaviour.
 *
 * The per-nation counter is `g_nation_colony_count[nation]++` and compiles to a single
 * `INC BYTE`. CheatMenuColonist's is spelled out as `x = x + 1` and compiles to a load, an
 * 8-bit increment and a store with the segment register reloaded between. Same operation,
 * same kind of byte array, two spellings, and the object code tells them apart -- so
 * the choice is recoverable and was not the same programmer's habit twice.
 *
 * And `g_cur_colony_rec` is declared NEAR here: `LES BX,[0x4b9e]` with no segment
 * override, reading a pointer out of DGROUP through DS. Other matched files declare
 * the same variable `Colony * ` and match that way, because in the large model
 * `` on a DGROUP variable forces an explicit segment load that `` does not need.
 * Both are byte-matched, so this is a real PER-MODULE declaration difference, the same
 * shape as the per-module flag differences in rules 28, 30 and 37 -- and equally invisible
 * until a reconstruction gets it wrong.
 *
 * `site_get` returns an `unsigned char`: the shipped `MOV AH,0` before the bit test
 * is the promotion, and declared `int` the instruction disappears.
 *
 * Ten callees arrive as the compiler's own 4-byte PUSH CS / CALL NEAR and are defined
 * here as stubs.
 */

#include "colonization.h"

#include "fathers.h"
#include "building.h"
#include "nations.h"
#include "control.h"
#include "jobs.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern unsigned char *feature_loc(int x, int y);
extern unsigned char site_get(int x, int y);
extern unsigned char continent_at(int x, int y);
extern int has_father(int nation, int bit);
extern void coronado_colony(int colony, int nation);

extern int water_access(int x, int y);

extern void set_colony(int colony);
extern void custom_set(int bit, int on);
extern void set_building(int bit, int on);
extern void compute_colony_parameters(void);
extern void colony_indian_table(void);
extern int guy_number_of_unit(int u);
extern void set_job(int colonist, int job);
extern void set_specialty(int a, int b);

int create_colony(int nation, int x, int y, int unit)
{
    int colonist;
    int i;
    register int col;
    register int u;

    u = unit;
    col = -1;
    if (g_game.colony_count >= 0x30) {
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
            show_text_dialog_1018_b3d1("TOOMANYCOLONIES");
        goto done;
    }
    if (on_map(x, y) == 0)
        goto done;
    col = g_game.colony_count;
    g_game.colony_count++;
    g_nation_colony_count[nation]++;
    set_colony(col);
    *feature_loc(x, y) |= 2;
    g_cur_colony_rec->nation = (unsigned char)nation;
    g_cur_colony_rec->x = (unsigned char)x;
    g_cur_colony_rec->y = (unsigned char)y;
    g_cur_colony_rec->ai_flags = 0;
    g_cur_colony_rec->status = 0;
    g_cur_colony_rec->ai_plan_flags = 0;
    g_cur_colony_rec->pop = 0;
    g_cur_colony_rec->hammers = 0;
    g_cur_colony_rec->warehouses = 0;
    g_cur_colony_rec->capitols = 0;
    g_cur_colony_rec->bonus_hammers = 0;
    g_cur_colony_rec->mine = 0;
    g_cur_colony_rec->changed = 0;
    g_cur_colony_rec->ai_idle_turns = 0;
    g_cur_colony_rec->wanted_goods = 0xff;
    g_cur_colony_rec->want_defenders = 0;
    g_cur_colony_rec->turns_since_ship = 0;
    g_cur_colony_rec->bells_cap = 100;
    g_cur_colony_rec->bells = 0;
    memset_rtl(g_cur_colony_rec->flags2, 0, 2);
    custom_set(1, 1);
    custom_set(2, 1);
    custom_set(3, 1);
    custom_set(4, 1);
    custom_set(6, 1);
    custom_set(7, 1);
    custom_set(9, 1);
    custom_set(0xa, 1);
    custom_set(0xb, 1);
    custom_set(0xc, 1);
    memset_rtl(g_cur_colony_rec->flags, 0, 6);
    set_building(BUILDING_CARPENTERS_SHOP, 1);
    set_building(BUILDING_TOWN_HALL, 1);
    compute_colony_parameters();
    memset_rtl(g_cur_colony_rec->stock, 0, 0x20);
    memset_rtl(g_cur_colony_rec->slots, -1, 0x14);
    if (u >= 0) {
        colonist = guy_number_of_unit(u);
        if (colonist >= 0) {
            set_job(colonist, 0);
        } else {
            g_colonies[col].pop = 1;
            set_job(0, 0);
            set_specialty(0, JOB_COLONIST_ALT);
        }
    }
    colony_indian_table();
    compute_colony_parameters();
    for (i = 0; i < 4; i++) {
        g_cur_colony_rec->hist_pop[i] = 1;
        g_cur_colony_rec->hist_bld[i] = 0;
        if (i >= 4 || (site_get(x, y) & (0x10 << i)))
            g_cur_colony_rec->hist_pop[i] = 1;
    }
    if (water_access(x, y) && continent_at(g_ocean_x, g_ocean_y) == 1)
        g_colonies[col].status |= 0x40;
    g_cur_colony_rec->item = 0xf;
    if (g_cur_colony_rec->status & 0x40)
        g_cur_colony_rec->item = 6;
    for (i = 0; i < 4; i++) {
        if (has_father(i, FATHER_FRANCISCO_DE_CORONADO))
            coronado_colony(g_cur_colony_idx, i);
    }
done:
    return col;
}
