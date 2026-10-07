/* start_indians: candidate for 1038:0ddf (2707 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE WHOLE NATIVE WORLD, BUILT AT NEW GAME -- the name understates it. Loading the TRIBES table is
 * the first ten lines; the rest places every village on the map, gives each one a brave, and totals up
 * the mountains that decide how rich the tribe's treasure will be. `start_new_game` calls it
 * once.
 *
 * THE TABLE. NAMES/TRIBES has one row per tribe -- "Incas, Inca, Jewelled Relics, 3, 97" -- and this
 * reads four numbers from each and KEEPS ONLY THE LAST, into +2 of the tribe record. The first three
 * parses are called for their side effect on the parser and their results dropped. Everything else in
 * the record is then cleared, including the sixteen goods-drift words, the four mood bytes and the
 * twelve at +0x3a; the four words at +0x46, one per European nation, get `rand(0, 14)` plus twice the difficulty for
 * each nation whose controller byte SEG20:0x7841 -- +1 of the 52-byte record at 0x7840 -- is zero.
 *
 * PLACING THE CAPITALS, on a generated map, is a rejection loop of up to twelve thousand tries per
 * tribe, and the rejection tests LOOSEN AS IT GOES: the minimum distance from the nearest existing
 * village starts at 90 and falls by one every four tries, a village closer than 8 squares is allowed
 * only after (8 - distance) thousand tries, the first two tribes must be at least x*8 tries from the
 * left edge, and a coarse cell that is already taken is allowed only after ten thousand. So the first
 * tribes get the best land and the last ones take what is left.
 *
 * ON A SCRIPTED MAP (SEG20:0x778a set) the capitals come from a TEXT section named TRIBE + the tribe's
 * own name, upper-cased -- TRIBEINCA, TRIBEAZTEC and so on -- as a list of coordinate pairs ending at
 * the first non-positive x. Each listed square is jittered by TWO `rand(-1, 1)` draws per axis, which
 * is a triangular distribution centred on the scripted square, and retried a hundred times against
 * terrain and a minimum distance that falls from 3 to 2 to 1 as the tries run out. The first village
 * of each tribe gets bit 2 of its flags: it is the capital.
 *
 * SCATTERING THE REST is a random walk. Pick a tribe that already has a village, start at its capital's
 * coarse cell, and step in random directions until an empty cell turns up or the walk leaves the 15 by
 * 18 grid. In an empty cell, every square of the inner 3 by 3 of the 5 by 5 block is tested -- right
 * terrain, no unit or settlement (MAP1_UNIT | MAP1_SETTLEMENT), and none on any of the nine squares around it -- and one of
 * the survivors is picked at random. It keeps going until 270 villages exist, or eight times that many
 * tries have been spent, or the settlement count reaches 84.
 *
 * THEN one brave per settlement, placed within two squares of it on land of the same land mass, and
 * finally every settlement's 5 by 5 is scanned for MOUNTAINS, each adding the tribe's level to the
 * counter at +0xc -- which is what makes a village in the Andes worth raiding.
 *
 * SHIPPED ODDITIES. The scripted-village counter is incremented and never initialised or read. The
 * `i < 4` test inside `for (i = 0; i < 4; i++)` can never fail. And `find_close_village`'s
 * return value is stored twice and never used -- the distance is read from DGROUP:0x4bc6, which the
 * call leaves behind.
 *
 * WHAT THE BYTES SETTLED: nearly every rejection test is a `continue`, not an `&&` chain -- a short
 * conditional jump over a long one, rule 12 -- and the one exception is the walk's bounds test, which
 * is an `||` chain ending in `break` (all four tests reach the same `JMP SHORT`). `g_cur_tribe_rec` and
 * `g_cur_settlement_rec` are declared FAR in this module where module 1030 declares them , so both
 * go through `MOV ES,SEG34` first (rule 9). `x * 8` and `target * 8` are `SHL 3` and `g_game.difficulty * 2`
 * is `ADD AX,AX`. The scatter loop is a `while`, not a `do`, so it can place nothing at all.
 *
 * `g_game.america_map` (SEG20:0x778a) is what the two `== 0` tests here mean: it is zero for a
 * GENERATED New World and non-zero for the fixed AMERICA map, so the eight tribes and the extra
 * settlements are placed at random only when there is no historical placement to use. BEGINMENU
 * names the three options the flag comes from; see `start_new_game`.
 *
 * TRIBE+0xc IS THE SILVER ENDOWMENT, which is why it is `silver_base` and not `fc`.
 * The last loop here walks the 5x5 around every native settlement and adds the TRIBE'S
 * LEVEL for each TERRAIN_MOUNTAINS square, and `indian_supply_and_demand` has the only read:
 * `g_native_sell[GOODS_SILVER] = silver_base / settlement_count`. So it is a tribe-wide
 * mountain tally, divided back out per village, and it sits beside the LOCAL mountain
 * count that same function keeps for its `mountains << 2` bonus.
 *
 * The four numbers per TRIBES line are worth stating too: the first three are the text
 * fields (name, adjective, treasure -- the same three `g_tribe` holds as word ids) which
 * `text_item_number` consumes and discards, and the fourth is the LEVEL. It reads back
 * against the LEVELS resource exactly: Incas 3 "Civilized", Aztecs 2 "Advanced",
 * Arawaks/Iroquois/Cherokee 1 "Agrarian", Apache/Sioux/Tupi 0 "Semi-Nomadic".
 *
 * RENAMED 2026-09-26 from load_tribe_table: the Mac build's start_indians(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (callee continent_at; callee create_village; callee feature_get; callee map_get; callee on_map), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "settle.h"
#include "control.h"
#include "units.h"
#include "terrain.h"
#include "records.h"

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern GameState g_game;   /* SEG20:0x7782 */

extern void random_activate(int seed);
extern int text_open(void *unused, char *id);
extern char *text_get(void);
extern int text_item_number(void);
extern char *strings(int n);

extern int build_check(int flag);
extern int terrain_is_ocean(int x, int y);

extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);
extern unsigned char continent_at(int x, int y);
extern int create_village(int kind, int x, int y);
/* 5-byte NOP form, so extern (rule 47) */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void set_tribe(int tribe);
extern void set_village(int s);
extern int find_close_village(int x, int y, int nation, int owner);

void start_indians(void)
{
    register int terrain;
    register int direction;
    int tribe;
    int s;
    int ok;
    int i;
    int tries;
    int near1;
    int ax2;
    int ay2;
    int scripted;
    int x;
    int y;
    int sx;
    int sy;
    int ddx;
    int ddy;
    int more;
    int region;
    int blocked;
    int want;
    int dist;
    int count;
    int target;
    int dir;
    int ncand;
    int first;
    char buf[0x50];

    random_activate(g_random_seed);
    text_open("NAMES", "TRIBES");
    target = 0x10e;
    memset_rtl(g_sketch, 0, target);
    count = 0;
    for (tribe = 0; tribe < 8; tribe++)
        g_tribe_settlement_count[tribe] = 0;
    for (tribe = 0; tribe < 8; tribe++) {
        set_tribe(tribe);
        text_get();
        text_item_number();
        text_item_number();
        text_item_number();
        g_cur_tribe_rec->level = text_item_number();
        g_cur_tribe_rec->x = g_cur_tribe_rec->y = 1;
        g_cur_tribe_rec->f4 = 0;
        g_cur_tribe_rec->count = 0;
        g_cur_tribe_rec->f6 = 0;
        g_cur_tribe_rec->muskets = 0;
        g_cur_tribe_rec->horse_herds = 0;
        g_cur_tribe_rec->horses = 0;
        g_cur_tribe_rec->silver_base = 0;
        for (i = 0; i < 4; i++) {
            g_cur_tribe_rec->anger[i] = rand_range(0, 14)
                + (i < 4 && g_nation_text[i].control == CONTROL_HUMAN ? g_game.difficulty * 2 : 0);
            g_cur_tribe_rec->calm[i] = 0;
        }
        for (i = 0; i < 0xc; i++)
            g_cur_tribe_rec->b3a[i] = 0;
        for (i = 0; i < 0x10; i++)
            g_cur_tribe_rec->goods_drift[i] = 0;
    }
    build_check(0);
    for (tribe = 0; tribe < 8; tribe++) {
        tries = 0;
        build_check(0);
        if (g_game.america_map == 0) {
            ok = 0;
            tries = 0;
            do {
                tries++;
                x = rand_range(8, g_map_size.w - 8);
                y = rand_range(12, g_map_size.h - 12);
                if (terrain_is_ocean(x, y))
                    continue;
                if (map_get(x, y) & MAP0_HILLY)
                    continue;
                near1 = find_close_village(x, y, -1, -1);
                dist = g_nearest_dist;
                if (dist == 0)
                    continue;
                if (90 - (tries >> 2) > dist)
                    continue;
                if (dist < 8 && (8 - dist) * 1000 > tries)
                    continue;
                if (tribe < 2 && x * 8 > tries)
                    continue;
                if (g_sketch[x / 5 * 18 + y / 5] != 0 && tries < 10000)
                    continue;
                ok = 1;
            } while (ok == 0 && tries < 12000);
            if (ok != 0) {
                g_tribes[tribe].x = x;
                g_tribes[tribe].y = y;
                s = create_village(tribe + 4, x, y);
                set_village(s);
                g_cur_settlement_rec->flags |= SETTLEMENT_CAPITAL;
                g_tribe_settlement_count[tribe]++;
                g_sketch[x / 5 * 18 + y / 5] = 1;
                count++;
            }
        } else {
            STRCPY(buf, strings(g_tribe[tribe].adjective));
            STRUPR(buf);
            if (text_open("TRIBE", buf) != 0)
                continue;
            first = 1;
            more = 1;
            do {
                text_get();
                x = text_item_number();
                y = text_item_number();
                if (x > 0) {
                    tries = 0;
                    do {
                        tries++;
                        ddx = rand_range(-1, 1) + rand_range(-1, 1);
                        ddy = rand_range(-1, 1) + rand_range(-1, 1);
                        sx = x + ddx;
                        sy = y + ddy;
                        ok = 0;
                        if (!on_map(sx, sy))
                            continue;
                        if (feature_get(sx, sy) & (MAP1_UNIT | MAP1_SETTLEMENT))
                            continue;
                        terrain = terrain_at(sx, sy);
                        if (terrain >= TERRAIN_ARCTIC)
                            continue;
                        terrain = terrain & 7;
                        if (!((terrain >= TERRAIN_PLAINS && terrain <= TERRAIN_MARSH) || terrain == TERRAIN_TUNDRA))
                            continue;
                        blocked = find_close_village(sx, sy, -1, -1);
                        if (tries < 0x21)
                            want = 3;
                        else if (tries < 0x42)
                            want = 2;
                        else
                            want = 1;
                        if (g_nearest_dist <= want)
                            continue;
                        ok = 1;
                    } while (ok == 0 && tries < 100);
                    if (ok != 0) {
                        s = create_village(tribe + 4, sx, sy);
                        if (first != 0) {
                            g_settlements[s].flags |= SETTLEMENT_CAPITAL;
                            first = 0;
                        }
                        scripted++;
                    }
                } else
                    more = 0;
            } while (more != 0);
        }
    }
    if (g_game.america_map == 0) {
        tries = 0;
        while (count < target && target * 8 > tries && g_game.settlement_count < 0x54) {
            do
                tribe = rand_range(0, 7);
            while (g_tribe_settlement_count[tribe] == 0);
            x = g_tribes[tribe].x / 5;
            y = g_tribes[tribe].y / 5;
            ok = 0;
            tries++;
            do {
                dir = rand_range(0, 7);
                x += g_dir_dx[dir];
                y += g_dir_dy[dir];
                if (x < 0 || x >= 15 || y < 0 || y >= 18)
                    break;
                if (g_sketch[x * 18 + y] != 0)
                    continue;
                ok = 1;
            } while (ok == 0);
            if (ok == 0)
                continue;
            ncand = 0;
            x = x * 5;
            y = y * 5;
            for (sy = y + 1; y + 4 > sy; sy++) {
                for (sx = x + 1; x + 4 > sx; sx++) {
                    if (on_map(sx, sy)) {
                        if (feature_get(sx, sy) & (MAP1_UNIT | MAP1_SETTLEMENT))
                            continue;
                        terrain = terrain_at(sx, sy);
                        if (terrain >= TERRAIN_ARCTIC)
                            continue;
                        terrain = terrain & 7;
                        if (!((terrain >= TERRAIN_PLAINS && terrain <= TERRAIN_MARSH) || terrain == TERRAIN_TUNDRA))
                            continue;
                        blocked = 0;
                        for (direction = 0; blocked == 0 && direction < 9; direction++) {
                            ax2 = g_dir_dx[direction] + sx;
                            ay2 = g_dir_dy[direction] + sy;
                            if (feature_get(ax2, ay2) & (MAP1_UNIT | MAP1_SETTLEMENT))
                                blocked = 1;
                        }
                        if (blocked != 0)
                            continue;
                        g_queue_x[ncand] = sx;
                        g_queue_y[ncand] = sy;
                        ncand++;
                    }
                }
            }
            if (ncand != 0) {
                s = rand_range(0, ncand - 1);
                sx = g_queue_x[s];
                sy = g_queue_y[s];
                find_close_village(sx, sy, -1, -1);
                s = create_village(g_cur_tribe_nation, sx, sy);
            }
            g_sketch[x / 5 * 18 + y / 5] = 1;
            count++;
            build_check(0);
        }
    }
    for (i = 0; i < g_game.settlement_count; i++) {
        tries = 0;
        region = continent_at(g_settlements[i].x, g_settlements[i].y);
        do {
            x = rand_range(-2, 2) + g_settlements[i].x;
            y = rand_range(-2, 2) + g_settlements[i].y;
            ok = on_map(x, y);
            if (ok != 0) {
                if (continent_at(x, y) != (unsigned char)region)
                    ok = 0;
                if (terrain_is_ocean(x, y))
                    ok = 0;
                if (feature_get(x, y) & (MAP1_UNIT | MAP1_SETTLEMENT))
                    ok = 0;
            }
            tries++;
        } while (ok == 0 && tries < 100);
        if (ok != 0) {
            s = create_unit(UNIT_BRAVES, g_settlements[i].owner, x, y);
            if (s >= 0)
                g_units[s].home = i;
        }
        build_check(0);
    }
    for (i = 0; i < g_game.settlement_count; i++) {
        set_village(i);
        for (y = g_cur_settlement_rec->y - 2; g_cur_settlement_rec->y + 2 >= y; y++) {
            for (x = g_cur_settlement_rec->x - 2; g_cur_settlement_rec->x + 2 >= x; x++) {
                if (on_map(x, y)) {
                    terrain = terrain_at(x, y);
                    if (terrain == TERRAIN_MOUNTAINS)
                        g_cur_tribe_rec->silver_base += g_cur_tribe_rec->level;
                }
            }
        }
    }
}
