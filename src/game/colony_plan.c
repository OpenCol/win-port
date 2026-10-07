/* PORT: this file's own declarations of build_another_colony, build_me_a removed: the header declares them from the definition (W1 step 3). */
/* colony_plan: candidate for 1018:04e0 (13365 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: colony_plan
 * name-source: sibling-build
 *
 * ONE COMPUTER-RUN COLONY'S ENTIRE TURN OF THINKING -- the largest function in the game
 * outside the window procedures, and the one that decides, for a single colony, who stands
 * where, what gets built and whether anybody is bought. It runs in nine stages:
 *
 *   1. SURVEY. `set_colony` makes the colony current, then the ring level and tile
 *      count come from `colony_map_status` and the eight-per-ring table at
 *      DGROUP:0xc01. Two idle counters at +0x8c and +0x8f tick up, capped at 0x7f.
 *   2. THREAT. Every square within five of the colony is walked; each foreign unit on it
 *      contributes `get_combat` weighted by `(8 - xy_dist) / 8`, halved
 *      again if a colony or village stands on the square. A European owner whose NationText
 *      control byte is 0 -- the human -- counts half again as much; a native owner counts
 *      nothing unless its tribe's attitude is at least 0x19 AND that tribe's anger at us has
 *      passed 0x80. Units 0xd..0x12 (the six ships; the wagon train, 0xc, is not skipped) are skipped throughout. The
 *      total, divided by the colony's fortification level (how many of stockade, fort, fortress it has) plus one but never below
 *      min(0x10, threat), becomes the garrison the colony wants: `want_defenders_threat`
 *      (+0x1e) keeps the raw eighth, `want_defenders` (+0x8e) the clamped figure, never
 *      more than half the population and never less than one when an enemy is actually
 *      adjacent. Both are COUNTDOWNS: plan_strategy spends them as it assigns
 *      artillery, soldiers and dragoons to the colony, and plan_move reads the raw one
 *      to decide whether a dragoon may still be released to a mission.
 *   3. TERRAIN. The worked ring is classified into water, forest, prime forest, improved,
 *      unimproved, plowed and unplowed counts, and the results are packed into the flag byte at
 *      +0x1b: 0x80 "there is work for a pioneer", 0x20 "there is forest worth clearing",
 *      0x40 "we want soldiers", 0x10 "we want more people", 8 "we are under our unit quota",
 *      4 "we are over it". The quota itself is `(pop * 3 / 2) - (per-nation byte + turn/128)`
 *      divided by a weight that starts at the same per-nation byte plus five.
 *   4. STANDING ORDERS. Colonists outside the colony (indices pop..pop+g_tile_colonists) are pulled off
 *      jobs 0x15/0x17 (soldier), 0x14 (pioneer) and 0x16 (scout) when the reason they were
 *      given them has gone away; the loop repeats until a pass changes nothing. Then one
 *      colonist may be PROMOTED into a new standing order, scored +4 for already having the
 *      specialty, +1 for having none, +1/+2 for an indentured servant (0x19) or a criminal (0x1a), a convert (0x1b) is skipped, and -99 -- only when soldiers are wanted and flag 0x40 is clear -- for being an
 *      expert in something else.
 *   5. EXPORTS. Five `ask_for_cargo` calls pick the one goods the colony asks to have brought in (`wanted_goods`, +0x8d) among muskets,
 *      trade goods, horses and tools. If nobody is on garrison duty and muskets have piled up
 *      past 0xc8 the surplus is banked as a Europe credit at Nation+0x49 instead.
 *   6. PIONEER WORK. With the 0x80 flag up and twenty tools in store, every ring tile is
 *      scored by terrain value, doubled for the job the occupant is actually doing, and
 *      penalised by half of four-minus-attitude when a native tribe claims it -- four times as much (twice four-minus-attitude)
 *      while the treasury is under 2000. The winner gets a throwaway unit of type
 *      `g_unit_types[UNIT_PIONEERS].icon` (the byte at SEG20:0x7ae6) with hit points 0x63, which clears the forest or builds the road, and
 *      `blow_fake_unit` takes it away again. Twenty tools are spent either way.
 *   7. WHO WORKS WHERE. Every colonist is stripped to job 0x12 (idle) and the twenty cells at
 *      +0x70 are cleared, then filled in priority order: food first while the colony is
 *      small for its land, short of food or breeding horses, then the outdoor jobs by specialty, then lumber, then the indoor
 *      jobs whose `job_available` says yes and whose goods will not overflow, three
 *      to a building. Whatever is left over is scored across jobs 9..0x12 and placed by the
 *      best score; a colonist whose best outdoor pick yields nothing falls to 0xd (carpenter), or to 0x10 (preacher) when there is a church with room.
 *   8. WHAT GETS BUILT. A single chain of thirty `try_to_build` tests, in
 *      strict priority: docks when the ring is full, then the stockade, the stables once there are
 *      horses, the first warehouse expansion at one level per six people, the custom house, the schoolhouse,
 *      the armory once muskets are wanted, the church, the lumber mill, the fort, the blacksmith's shop, more
 *      warehouse expansions, the newspaper, the college, the university, the fortress, the shipyard once any
 *      level-3 building is up, and the artillery when there is none.
 *      The first one that returns 0 has taken the build slot, and the chain stops.
 *   9. BUYING A TEACHER. With a schoolhouse of any level and the idle counter past four times the
 *      level count, one colonist may be handed a free specialty, and with the tax rate at or
 *      under 0x19 a farmer or fisherman is bought outright for the price in `g_spec_recs`.
 *
 * FOUR SHIPPED BUGS, all reproduced because the bytes require them:
 *
 *   - THE PRICE PAID IS NOT THE PRICE CHECKED. Stage 9 tests `g_spec_recs[0].price` against
 *     the treasury and then subtracts `g_spec_recs[pick].price`, where `pick` is a COLONIST
 *     INDEX, not a record index. A colony with more than one colonist can therefore buy a
 *     teacher it cannot afford, or be charged the wrong price for one it can.
 *   - THE PROMOTION READS A STALE LOOP VARIABLE. Stage 4 decides whether to wipe the winner's
 *     specialty with `any_spec(spc) && spc != want_spec`, but `spc` there is whatever
 *     the LAST colonist in the scoring loop happened to be, not the winner's. The winner is
 *     `bestidx`; `spc` belongs to `pop - 1`.
 *   - THE INDOOR SCORE READS EIGHT BYTES BELOW THE GOODS TABLE. The goods-below-0x10 branch of the
 *     job score subtracts a byte at SEG30:0x8 using the SAME `owner * 0x10 + goods` index the
 *     line above uses against the goods-value table at SEG30:0x10. For every nation and goods
 *     it is reading `g_goods_value[owner][goods - 8]`, which for the low goods is the eight
 *     bytes of unrelated scratch that `ColChild_DragIcon` uses as a rectangle. Spelled
 *     `(&g_goods_value[0][-8])[owner * 16 + goods]`: the shipped fixup names g_goods_value (tools/
 *     data_owners.py), and that form folds the -8 into the relocation, where `[goods - 8]`
 *     becomes a run-time `ADD BX,-8`, which is not what shipped.
 *   - THE BELL TEST IS WRITTEN TWICE. `g_prod[PROD_BELLS] >= 0x18` and `g_prod[PROD_BELLS] >= 4` both guard
 *     the same `try_to_build(0x14)`, so the first test can never decide anything the
 *     second would not.
 *
 * WHAT THE BYTES SETTLED, function by function. Thirty-one divergences were fixed to get
 * here; these are the ones that taught something new:
 *
 *   - THE SIZE IS 13365, NOT THE CENSUS'S 13344. A nine-entry jump table for the
 *     `switch (pass)` at stage 7 sits after the epilogue, indexed `CS:[BX+0x3903]`. Matching
 *     at the census size reports DIFFERS at the last byte of the body.
 *   - THE FRAME IS `SUB SP,0x1b6`: two register variables (`i` in SI, `w` in DI) and then
 *     eighty-six word scalars in one exact declaration order, followed by
 *     `assigned[32]`, `spec[32]`, `verdict[19]`, `list[25]`, `count[25]`. Borland drops an
 *     unreferenced local, so the frame cannot be measured until the whole body is written.
 *   - TWENTY-SIX of the thirty-three same-segment callees use `NOP + PUSH CS + CALL `, so
 *     they are plain `extern`s; the other seven use the four-byte form and have to be in-file
 *     stubs (rule 47).
 *   - `w += tories + 4` is TWO compound adds in the shipped (`ADD DI,[tories]` then
 *     `ADD DI,4`), not one load-add-add-store.
 *   - `w >>= 1` gives `SAR DI,1` in place; `w = w >> 1` gives load/shift/store. The same
 *     function uses BOTH forms, four of the first and one of the second, and each site has
 *     to be spelled the way it shipped (rule 60).
 *   - THE PER-NATION WORD AT SEG20:0xd9d6 AND THE BELL COUNT AT SEG21:0x146 ARE UNSIGNED.
 *     Their compares are `JNA`/`JNC`, not `JNG`/`JL`, which is visible nowhere else.
 *   - `warehouses < want_warehouses` is a PURE BYTE COMPARE (`CMP AL,[BP-0x76]`). Only casting the int
 *     to `(unsigned char)` produces it; without the cast Borland promotes with `MOV AH,0`.
 *   - THE JOB PICK IS A TERNARY. `job = newspec >= 0 ? newspec : get_job(pick)` stores
 *     to the slot ONCE at the join; an if/else stores in each arm.
 *   - FIVE `continue`s HAD TO BECOME NESTED `if`s. Rule 12 both ways: a direct short JCC to
 *     the loop end is a structured `if`, a short JCC over a `JMP` is a `continue`.
 *   - ARGUMENTS ARE EVALUATED LEFT TO RIGHT and the left result is PUSHED, so
 *     `job_count(0) + job_count(8)` and the reverse are distinguishable.
 *
 * `g_goods_input` (SEG21:0xa6) is what a job CONSUMES. `quota = g_goods_input[spec[i]]` and
 * then `g_cur_colony_rec->stock[quota]`, `g_prod[quota]` and `g_shortfall[quota]` -- all three
 * goods-indexed -- so the table turns a colonist's specialty into the raw good that feeds it,
 * and the AI wants that job only when the input is not already over-supplied.
 *
 * RENAMED 2026-09-26 from ai_manage_colony: the Mac build's colony_plan(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee has_father; callee land_region_at; callee minimax; callee on_map; callee set_colony), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "fathers.h"
#include "nations.h"
#include "control.h"
#include "colflag.h"
#include "aicol.h"
#include "jobs.h"
#include "goods.h"
#include "gameflag.h"
#include "building.h"
#include "terrain.h"
#include "tribes.h"
#include "units.h"
#include "region.h"
#define ABS(v) ((v) > 0 ? (v) : ~(v) + 1)
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_consumption[];                    /* SEG21:0x164 */
extern int g_deficit[];                  /* SEG21:0x18c */
extern int g_shortfall[];                /* SEG21:0x1b4 */

extern int land_region_at(int x, int y);
extern int find_close_village(int x, int y, int nation, int land);
extern int pissed(int tribe, int n);

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern int get_combat(int u, int attacking);
extern int is_city(int x, int y);
extern int is_anything(int x, int y);
extern int xy_dist(int dx, int dy);

extern unsigned char feature_get(int x, int y);
extern int resource_at(int x, int y);
extern int terrain_is_ocean(int x, int y);

extern int has_father(int nation, int bit);
extern int treaty_on(int nation, int other, int m);
extern void traffic_from_europe(int g, int n);
extern int make_fake_unit(int icon, int x, int y, int owner, int strength);
extern void blow_fake_unit(void);
extern void perform_orders_clear(int u);
extern void perform_orders_road(int u);
extern void raise_taxes(char *key, int delta);

extern void set_colony(int colony);
extern void colony_indian_table(void);
extern void compute_colony_parameters(void);
extern int get_warehouse(void);
extern int colony_map_status(void);
extern int colony_max_size(void);
extern int colony_rebel(void);
extern int building_level(int bit);
extern int has_building(int bit);
extern int get_specialty(int u);
extern void set_specialty(int u, int s);
extern void set_progress(int u, int progress);
extern int get_job(int u);
extern int any_spec(int job);
extern int set_job(int u, int job);
extern void reset_colony_units(void);
extern void colony_field_table(void);
extern int job_available(int job);
extern void compute_colony_yield(void);
extern int preassign_colonist_to_field(int u, int want);
extern void pick_up_colonist(int u);
extern int compute_colonist_yield(int u, int *g);
extern int top_building(int building);
extern int job_count(int job);
extern int specialty_count(int spec);
extern int cargo_free(int g);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int try_connect(void);
extern "C" void ask_for_cargo(int g, int flag);
extern "C" int try_to_build(int item);
extern "C" int needs_a_building(int idx);

extern "C" void colony_plan(int colony)
{
    /* declared HERE, at block scope: this function reaches them with a segment
       fixup while the rest of segment 4 reads the same globals through DS, and one
       C++ module can hold both views only this way */

    register int i;
    register int w;
    int x;
    int y;
    int tx;
    int ty;
    int dx;
    int dy;
    int pass;
    int nation;
    int cap;
    int want;
    int best;
    int job;
    int k;
    int build_another;
    int val;
    int unworkable;
    int water;
    int bestidx;
    int good;
    int need;
    int ring;
    int tiles;
    int experts;
    int maxrank;
    int spc;
    int worked;
    int improved;
    int want_flag;
    int want_job;
    int terrain;
    int changed;
    int forest;
    int unimproved;
    int plane1;
    int plowed;
    int unplowed;
    int food_rich;
    int prime;
    int farmers;
    int farm_experts;
    int need_food_bld;
    int pick;
    int have_tools;
    int ok;
    int u;
    int forest_flag;
    int deco;
    int tval;
    int quota;
    int tmp2;
    int f68 = 0;
    int got5 = 0;
    int village;
    int village_dist;
    int region;
    int att;
    int soldiers;
    int want_warehouses;
    int threat;
    int adj_threat;
    int str;
    int want_spec;
    int wants_muskets;
    int mycount = 0;
    int limit;
    int limit2;
    int nlist;
    int pick2;
    int newspec;
    int school_level;
    int claim;
    int have3;
    int region_state;
    int small_pop;
    int horses_breeding;
    int either;
    int f9c;
    int artillery = 0;
    int lumber_supply;
    int tmp;
    int cost;
    int didwork;
    int sol;
    int tories;
    int wantcustom;
    int is_center;
    int assigned[32];
    int spec[32];
    int verdict[19];
    int list[25];
    int count[25];

    set_colony(colony);
    is_center = (g_cur_colony_rec->x == 0x2e && g_cur_colony_rec->y == 0x14);
    memset_rtl(&g_cur_colony_rec->flags2, 0, 2);
    colony_indian_table();
    compute_colony_parameters();
    cap = get_warehouse();
    ring = colony_map_status();
    tiles = g_ring_tile_count[ring - 1];
    if ((int)g_cur_colony_rec->ai_idle_turns < 0x7f)
        g_cur_colony_rec->ai_idle_turns++;
    if ((int)g_cur_colony_rec->turns_since_ship < 0x7f)
        g_cur_colony_rec->turns_since_ship++;
    have_tools = (g_cur_colony_rec->stock[GOODS_TOOLS] >= 0x14);
    x = g_cur_colony_rec->x;
    y = g_cur_colony_rec->y;
    nation = g_cur_colony_rec->nation;
    region = land_region_at(x, y);
    region_state = g_region_state[nation][region];
    soldiers = 0;
    threat = 0;
    adj_threat = 0;
    sol = colony_rebel();
    tories = (g_cur_colony_rec->pop * (100 - sol) + 0x32) / 100;
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        tories = 0;
    village = find_close_village(x, y, -1, region);
    village_dist = g_nearest_dist;
    if (village >= 0)
        att = pissed(g_cur_tribe, nation);
    for (dy = -5; dy <= 5; dy++) {
        for (dx = -5; dx <= 5; dx++) {
            tx = x + dx;
            ty = y + dy;
            if (on_map(tx, ty) == 0)
                continue;
            u = top_unit_in_square(tx, ty);
            if (u < 0)
                continue;
            if ((unsigned char)(g_units[u].flags & 0xf) == (unsigned char)nation)
                continue;
            for (; u >= 0; u = next_unit(u)) {
                str = get_combat(u, 1);
                if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                    continue;
                if ((unsigned char)(g_units[u].flags & 0xf) < 4) {
                    if (str <= 1)
                        str = 0;
                    if ((unsigned char)(g_units[u].flags & 0xf) < 4
                        && g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].control == CONTROL_HUMAN)
                        str += str >> 1;
                    if (str != 0 && ABS(dx) <= 1 && ABS(dy) <= 1)
                        adj_threat++;
                } else {
                    att = pissed((unsigned char)(g_units[u].flags & 0xf) - 4, nation);
                    if (att < 0x19)
                        str = 0;
                    if (g_settlements[g_units[u].home].anger[nation] < 0x80)
                        str = 0;
                }
                if (is_city(tx, ty) >= 0)
                    str = str >> 1;
                str = (8 - xy_dist(dx, dy)) * str >> 3;
                threat += str;
            }
        }
    }
    tmp = min_int_1018_b329(0x10, threat);
    threat = threat / (building_level(BUILDING_STOCKADE) + 1);
    threat = max_int_1018_b34f(threat, tmp);
    soldiers = threat >> 3;
    g_cur_colony_rec->want_defenders_threat = soldiers;
    soldiers = max_int_1018_b34f((g_cur_colony_rec->pop + g_tile_colonists - 1) / 2, soldiers);
    soldiers = min_int_1018_b329((g_cur_colony_rec->pop + g_tile_colonists) >> 1, soldiers);
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        soldiers++;
    if (adj_threat != 0 && g_cur_colony_rec->pop + g_tile_colonists > 1)
        soldiers = max_int_1018_b34f(1, soldiers);
    g_cur_colony_rec->want_defenders = soldiers;
    for (u = top_unit_in_square(x, y); u >= 0; u = next_unit(u)) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
            if ((int)g_units[u].home < 0)
                g_units[u].home = g_cur_colony_idx;
            if (g_unit_types[g_units[u].type].attack > 1 && soldiers != 0)
                soldiers--;
        }
    }
    for (u = 0; g_game.unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & 0xf) != (unsigned char)nation)
            continue;
        if ((unsigned char)g_units[u].home != (unsigned char)g_cur_colony_idx)
            continue;
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
            continue;
        if (g_unit_types[g_units[u].type].attack <= 1)
            continue;
        mycount++;
    }
    mycount -= g_cur_colony_rec->want_defenders - soldiers;
    val = 0;
    unworkable = 0;
    water = 0;
    worked = 0;
    improved = 0;
    forest = 0;
    unimproved = 0;
    food_rich = 0;
    prime = 0;
    plowed = 0;
    unplowed = 0;
    for (i = 0; i < tiles; i++) {
        x = g_cur_colony_rec->x + g_ring_dx[i];
        y = g_cur_colony_rec->y + g_ring_dy[i];
        if (on_map(x, y) == 0) {
            unworkable++;
            val++;
            continue;
        }
        terrain = terrain_at(x, y);
        if (terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE) {
            val++;
            water++;
            unworkable++;
            food_rich++;
        }
        if ((terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST) || (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC)) {
            forest++;
            val++;
            if (g_terrain_rows[terrain & 7].yield[GOODS_FOOD] >= 3)
                prime++;
        } else {
            if (g_terrain_rows[terrain].yield[GOODS_FOOD] >= 3)
                food_rich++;
            else if (g_terrain_rows[terrain].yield[GOODS_FOOD] < 2)
                val++;
        }
        plane1 = feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD);
        if ((int)g_cur_colony_rec->slots[i] >= 0) {
            worked++;
            if (plane1 == 0)
                unimproved++;
            if (plane1 != 0)
                improved++;
            if (terrain <= TERRAIN_SWAMP) {
                if (feature_get(x, y) & MAP1_PLOWED)
                    plowed++;
                else
                    unplowed++;
            }
        }
    }
    if (has_building(BUILDING_DOCKS))
        unworkable = 0;
    g_cur_colony_rec->ai_flags &= 7;
    if ((g_cur_colony_rec->status & COLONY_AI_SMALL) && (int)g_cur_colony_rec->pop < 0x20) {
        g_cur_colony_rec->ai_flags |= COLONY_AI_HAS_ROOM;
        g_cur_colony_rec->status &= ~COLONY_AI_SMALL;
    }
    if (soldiers > 0)
        g_cur_colony_rec->ai_flags |= COLONY_AI_HAS_SOLDIERS;
    quota = ((g_cur_colony_rec->pop + g_tile_colonists) * 3 >> 1)
            - (g_leader_traits[nation][2] + (g_game.turn_counter >> 7));
    w = g_leader_traits[nation][2] + 5;
    if (soldiers != 0)
        w++;
    if (region_state == 4)
        w--;
    if (region_state == 0)
        quota += 2;
    if (region_state == 3)
        quota++;
    limit = quota / w;
    if (g_region_flags[region] == 0 && region_state != 0)
        limit = 0;
    if (g_region_flags[region] & REGION_NATIVE_SETTLEMENT) {
        if (!(g_region_flags[region] & (REGION_RIVAL_UNITS | REGION_RIVAL_COLONY)) || nation == NATION_SPAIN) {
            if (g_nation_region_strength[nation][region] >= 2) {
                quota = g_military_power[nation] * 2;
                if (nation == NATION_SPAIN)
                    quota = quota * 2;
                tmp2 = g_nation_region_strength[nation][region] << 2;
                if (nation == NATION_SPAIN)
                    tmp2 = tmp2 * 2;
                if (g_tribe_strength[g_cur_tribe] <= quota
                    && g_tribe_region[g_cur_tribe][region] < tmp2
                    && (pissed(g_cur_tribe, nation) > TRIBE_ATTITUDE_MEDIUM || nation == NATION_SPAIN))
                    treaty_on(nation, g_cur_tribe_nation, 2);
            }
        } else
            limit = min_int_1018_b329(1, limit);
    }
    if (mycount < limit)
        g_cur_colony_rec->ai_flags |= COLONY_AI_UNDER_LIMIT;
    if ((limit > 1 ? 1 : 0) + limit < mycount)
        g_cur_colony_rec->ai_flags |= COLONY_AI_OVER_LIMIT;
    if (tiles - 1 <= val && forest > 1)
        g_cur_colony_rec->ai_flags |= 0xa0;
    if ((g_cur_colony_rec->pop + 3) >> 2 > food_rich && prime != 0 && forest > 1)
        g_cur_colony_rec->ai_flags |= 0xa0;
    if (unimproved != 0 || unplowed != 0)
        g_cur_colony_rec->ai_flags |= COLONY_AI_LAND_UNIMPROVED;
    build_another = build_another_colony(g_cur_colony_rec->nation);
    if ((int)g_cur_colony_rec->pop < 0x20
        && (char)(colony_max_size() + ring * 2)
           > (char)g_cur_colony_rec->pop
        && (int)g_cur_colony_rec->pop - ring * 2 < tiles - unworkable)
        g_cur_colony_rec->ai_flags |= COLONY_AI_HAS_ROOM;
    memset_rtl(list, 0, 0x32);
    memset_rtl(count, 0, 0x32);
    for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
        spc = get_specialty(i);
        if (any_spec(spc) == 0)
            spc = 0x13;
        list[spc]++;
    }
    reset_colony_units();
    if (g_tile_colonists != 0 && (g_cur_colony_rec->ai_flags & COLONY_AI_HAS_ROOM)) {
        do {
            changed = 0;
            for (i = (int)g_cur_colony_rec->pop;
                 changed == 0 && (int)g_cur_colony_rec->pop + g_tile_colonists > i
                 && (int)g_cur_colony_rec->pop < 0x20;
                 i++) {
                job = get_job(i);
                spc = get_specialty(i);
                if (job == JOB_SOLDIER || job == JOB_DRAGOON) {
                    if ((soldiers < 0 && !(g_cur_colony_rec->ai_flags & COLONY_AI_UNDER_LIMIT))
                        || ((any_spec(spc) && spc != 0x15)
                            && (list[19] != 0 || list[21] != 0))
                        || (g_cur_colony_rec->ai_flags & COLONY_AI_OVER_LIMIT)) {
                        set_job(i, 0x12);
                        soldiers++;
                        changed = 1;
                        g_cur_colony_rec->ai_flags &= ~COLONY_AI_OVER_LIMIT;
                        if (list[21] != 0)
                            list[21]--;
                        else if (list[19] != 0)
                            list[19]--;
                    }
                }
                if (job == JOB_PIONEER) {
                    if (((g_cur_colony_rec->ai_flags & COLONY_AI_LAND_UNIMPROVED) && have_tools == 0)
                        || region_state == 0) {
                        set_job(i, 0x12);
                        have_tools = 1;
                        changed = 1;
                    }
                }
                if (job == JOB_SCOUT) {
                    if (region_state == 0 || g_cur_colony_rec->stock[GOODS_HORSES] < 0x34) {
                        set_job(i, 0x12);
                        changed = 1;
                    }
                }
                if (job == JOB_COLONIST) {
                    set_job(i, 0x12);
                    changed = 1;
                }
            }
        } while (changed != 0);
    }
    if ((int)g_cur_colony_rec->pop > 1) {
        want_job = 0x13;
        want_flag = 0;
        if (g_cur_colony_rec->stock[GOODS_HORSES] >= 0x66) {
            if ((int)g_cur_colony_rec->pop >= 0xa
                || (char)colony_max_size()
                   <= (char)g_cur_colony_rec->pop) {
                if (!(g_cur_colony_rec->ai_flags & COLONY_AI_HAS_ROOM)) {
                    want_job = 0x16;
                    want_flag = 1;
                }
            }
        }
        wantcustom = (region_state == 0 && (int)g_cur_colony_rec->pop > 0xa
                      && rand_range(0, 3) == 0
                      && !(g_cur_colony_rec->ai_flags & COLONY_AI_HAS_ROOM));
        if (wantcustom != 0 && g_unit_tally[nation][UNIT_PIONEERS] == 0 && build_another != 0
            && g_cur_colony_rec->stock[GOODS_TOOLS] >= 0x14) {
            want_job = 0x14;
            want_flag = 1;
        }
        if ((g_cur_colony_rec->ai_flags & (COLONY_AI_UNDER_LIMIT | COLONY_AI_HAS_SOLDIERS)) || wantcustom != 0) {
            if (g_cur_colony_rec->stock[GOODS_MUSKETS] >= 0x32) {
                want_job = 0x15;
                if (g_cur_colony_rec->stock[GOODS_HORSES] >= 0x34)
                    want_job = 0x17;
                want_flag = 1;
            }
        }
        if (want_flag != 0) {
            want_spec = want_job;
            if (want_job == 0x17)
                want_spec = 0x15;
            best = -1;
            bestidx = -1;
            for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
                val = 0;
                spc = get_specialty(i);
                if (spc == want_spec)
                    val += 4;
                else if (any_spec(spc) == 0)
                    val++;
                else if (want_spec == 0x15 && !(g_cur_colony_rec->ai_flags & COLONY_AI_HAS_SOLDIERS))
                    val = -99;
                if (spc == 0x19)
                    val++;
                if (spc == 0x1a)
                    val += 2;
                if (spc == 0x1b)
                    continue;
                if (val >= best) {
                    best = val;
                    bestidx = i;
                }
            }
            if (bestidx >= 0) {
                if (any_spec(spc) && spc != want_spec)
                    set_specialty(bestidx, JOB_COLONIST_ALT);
                set_job(bestidx, want_job);
            }
        }
    }
    reset_colony_units();
    ask_for_cargo(0xf,
        g_leader_traits[nation][0] * 0x32 + 0x64 > g_cur_colony_rec->stock[GOODS_MUSKETS]);
    wants_muskets = ((int)g_cur_colony_rec->wanted_goods == 0xf);
    ask_for_cargo(0xd,
        (g_cur_colony_rec->stock[GOODS_TRADE_GOODS] < 0x64
         && g_goods_value[nation][0xd] <= 3
         && (g_cur_colony_rec->status & 0x20)));
    ask_for_cargo(8, g_cur_colony_rec->stock[GOODS_HORSES] < 0x32);
    ask_for_cargo(0xe,
        (have_tools == 0 && (g_cur_colony_rec->ai_flags & COLONY_AI_LAND_UNIMPROVED)));
    ask_for_cargo(0xf,
        ((soldiers > 0 && g_cur_colony_rec->stock[GOODS_MUSKETS] < 0x32)
         || ((int)g_cur_colony_rec->want_defenders == 1 && g_cur_colony_rec->stock[GOODS_MUSKETS] < 0x32
             && (int)g_cur_colony_rec->wanted_goods != 0xe)
         || ((g_cur_colony_rec->ai_flags & COLONY_AI_UNDER_LIMIT) && g_cur_colony_rec->stock[GOODS_MUSKETS] < 0x32
             && (int)g_cur_colony_rec->wanted_goods != 0xe)
         || (wants_muskets != 0 && (int)g_cur_colony_rec->wanted_goods == 0xf)));
    if (soldiers == 0 && g_cur_colony_rec->stock[GOODS_MUSKETS] >= 0xc8
        && g_cur_nation_rec->muskets < 0x14) {
        g_cur_nation_rec->muskets++;
        g_cur_colony_rec->stock[GOODS_MUSKETS] -= 0x32;
    }
    g_grid_marks_done = 0;
    colony_field_table();
    if (((g_cur_colony_rec->ai_flags & COLONY_AI_LAND_UNIMPROVED) || g_game.turn_counter % 10 == 0) && have_tools == 0) {
        cost = g_goods_value[g_cur_colony_rec->nation][0xe] * 0x14;
        if (cost <= g_cur_nation_rec->gold) {
            g_cur_nation_rec->gold -= cost;
            traffic_from_europe(0xe, 0x14);
            g_cur_colony_rec->stock[GOODS_TOOLS] += 0x14;
            have_tools = 1;
        }
    }
    didwork = 0;
    if (have_tools != 0 && g_game.turn_counter % 7 == 0 && try_connect() != 0) {
        g_cur_colony_rec->stock[GOODS_TOOLS] -=
            min_int_1018_b329(0x14, g_cur_colony_rec->stock[GOODS_TOOLS]);
        g_cur_colony_rec->ai_idle_turns = 0;
        didwork = 1;
    }
    if ((g_cur_colony_rec->ai_flags & COLONY_AI_LAND_UNIMPROVED) && have_tools != 0 && g_game.turn_counter % 7 != 0) {
        bestidx = -1;
        best = -1;
        for (k = 0; k < tiles; k++) {
            x = g_cur_colony_rec->x + g_ring_dx[k];
            y = g_cur_colony_rec->y + g_ring_dy[k];
            tx = g_ring_dx[k] + 2;
            ty = g_ring_dy[k] + 2;
            if (on_map(x, y) == 0)
                continue;
            if (terrain_is_ocean(x, y) != 0)
                continue;
            if (g_grid_marks[tx][ty] != 0)
                continue;
            terrain = terrain_at(x, y);
            deco = resource_at(x, y);
            tval = g_terrain_rows[terrain].value;
            if (deco != -1)
                tval = g_resource_value[deco];
            if ((g_cur_colony_rec->ai_flags & 0x20) && terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_ARCTIC)
                tval = tval * 2;
            else {
                if ((int)g_cur_colony_rec->slots[k] >= 0) {
                    job = get_job(g_cur_colony_rec->slots[k]);
                    if (job < 4) {
                        if (!(feature_get(x, y) & MAP1_PLOWED))
                            tval = tval * 2;
                    } else {
                        if (!(feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD)))
                            tval = tval * 2;
                    }
                }
                if ((feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD)) && (feature_get(x, y) & MAP1_PLOWED))
                    continue;
            }
            claim = g_tile_tribe[tx][ty];
            if (claim >= 0) {
                att = pissed(claim, nation);
                att = 4 - att;
                if (deco != -1)
                    att = att * 2;
                if ((g_region_flags[region] & REGION_NATIVE_SETTLEMENT) && !(g_cur_colony_rec->status & 0x20)) {
                    if (!(g_cur_colony_rec->ai_flags & 0x20))
                        continue;
                    att = att * 2;
                }
                if (g_cur_nation_rec->gold < 2000L)
                    tval = tval - att * 2;
                else
                    tval = tval - (att >> 1);
            }
            if (tval > best) {
                best = tval;
                bestidx = k;
            }
        }
        if (bestidx >= 0) {
            x = g_cur_colony_rec->x + g_ring_dx[bestidx];
            y = g_cur_colony_rec->y + g_ring_dy[bestidx];
            ok = 1;
            for (k = 0; k < 8; k++) {
                tx = g_dir_dx[k] + x;
                ty = g_dir_dy[k] + y;
                nation = is_anything(tx, ty);
                if (nation >= 0 && nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
                    ok = 0;
            }
            if (is_anything(x, y) == g_cur_colony_rec->nation)
                ok = 1;
            if (ok != 0) {
                terrain = terrain_at(x, y);
                val = g_terrain_rows[terrain].work_turns + 2;
                forest_flag = (((terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST)
                                || (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC))
                               && (g_cur_colony_rec->ai_flags & 0x20));
                if (forest_flag != 0)
                    val += 2;
                if ((char)g_cur_colony_rec->ai_idle_turns >= (char)val) {
                    u = make_fake_unit(g_unit_types[UNIT_PIONEERS].icon, x, y,
                                                     g_cur_colony_rec->nation, 0);
                    g_units[u].counter = 0x63;
                    if (forest_flag != 0) {
                        perform_orders_clear(u);
                        didwork = 1;
                        goto work_done;
                    } else {
                        job = -1;
                        if ((int)g_cur_colony_rec->slots[bestidx] >= 0)
                            job = get_job(g_cur_colony_rec->slots[bestidx]);
                        if (job >= 0 && job < 4 && !(feature_get(x, y) & MAP1_PLOWED)) {
                            perform_orders_clear(u);
                            didwork = 1;
                        } else if (job >= 4 && !(feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD))) {
                            perform_orders_road(u);
                            didwork = 1;
                        } else if (terrain >= TERRAIN_PLAINS && terrain <= TERRAIN_SWAMP) {
                            if (!(feature_get(x, y) & MAP1_PLOWED)) {
                                perform_orders_clear(u);
                                didwork = 1;
                            }
                        } else if (!(feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD))) {
                            perform_orders_road(u);
                            didwork = 1;
                        }
                    }
work_done:
                    blow_fake_unit();
                    if (didwork != 0) {
                        g_cur_colony_rec->stock[GOODS_TOOLS] -=
                            min_int_1018_b329(0x14, g_cur_colony_rec->stock[GOODS_TOOLS]);
                        g_cur_colony_rec->ai_idle_turns = 0;
                    }
                }
            }
        }
    }
    nation = g_cur_colony_rec->nation;
    for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
        assigned[i] = 0;
        spec[i] = get_specialty(i);
        set_progress(i, 0);
        set_job(i, 0x12);
    }
    for (i = 0; i < 0x14; i++)
        g_cur_colony_rec->slots[i] = -1;
    for (i = 0; i < 0x13; i++)
        verdict[i] = job_available(i);
    memset_rtl(g_cur_colony_rec->slots, -1, 0x14);
    compute_colony_yield();
    small_pop = ((int)g_cur_colony_rec->pop < tiles * 2);
    horses_breeding = (g_cur_colony_rec->stock[GOODS_HORSES] > 1
               && g_cur_colony_rec->stock[GOODS_HORSES] < cap);
    either = (small_pop != 0 || horses_breeding != 0);
    if (either == 0 && g_cur_colony_rec->stock[GOODS_FOOD] >= 0x4b)
        goto placed;
    for (i = 0; (int)g_cur_colony_rec->pop > i
                && (small_pop != 0 || g_prod[GOODS_FOOD] <= g_consumption[GOODS_FOOD] || horses_breeding != 0);
         i++) {
        if (assigned[i] != 0)
            continue;
        if (spec[i] == 0 || (spec[i] == 8 && has_building(BUILDING_DOCKS))) {
            if (preassign_colonist_to_field(i, spec[i]) == 0) {
                if (g_best_job_amount < 3) {
                    pick_up_colonist(i);
                    set_job(i, 0x12);
                    goto placed;
                }
                assigned[i] = 1;
                compute_colony_yield();
            }
        }
    }
    for (pass = 0; pass < 2; pass++) {
        for (i = 0; (int)g_cur_colony_rec->pop > i
                    && ((g_deficit[GOODS_FOOD] != 0
                         && (g_deficit[GOODS_FOOD] << 4) >= g_cur_colony_rec->stock[GOODS_FOOD])
                        || either != 0);
             i++) {
            if (assigned[i] != 0)
                continue;
            quota = ((g_deficit[GOODS_FOOD] << 4) < g_cur_colony_rec->stock[GOODS_FOOD]);
            if (any_spec(spec[i])) {
                if (quota != 0)
                    continue;
                if (pass == 0)
                    continue;
            }
            if (quota != 0 && pass == 0 && spec[i] != 0x1b)
                continue;
            if (preassign_colonist_to_field(i, -1) == 0) {
                if (g_best_job_amount < 3 || (quota != 0 && g_best_job_amount < 5)) {
                    pick_up_colonist(i);
                    set_job(i, 0x12);
                    goto placed;
                }
                assigned[i] = 1;
                compute_colony_yield();
            }
        }
    }
placed:
    g_ai_managing_colony = 1;
    if (unworkable != 0 && (int)g_cur_colony_rec->pop >= tiles - unworkable)
        goto after_outdoor;
    if ((char)colony_max_size() <= (char)g_cur_colony_rec->pop)
        goto after_outdoor;
    best = -1;
    bestidx = -1;
    for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
        if (assigned[i] != 0)
            continue;
        if (preassign_colonist_to_field(i, -2) == 0) {
            g_best_job_score = g_best_job_score << 2;
            if (any_spec(spec[i]) == 0)
                g_best_job_score++;
            if (spec[i] == g_wanted_specialty)
                g_best_job_score += 2;
            if (g_best_job_score > best && g_best_job_amount >= 2) {
                best = g_best_job_score;
                bestidx = i;
            }
        }
    }
    if (bestidx >= 0 && preassign_colonist_to_field(bestidx, -1) == 0) {
        assigned[bestidx] = 1;
        compute_colony_yield();
        if (get_job(bestidx) == 5)
            got5 = 1;
    }
after_outdoor:
    f9c = 0;
    if (g_cur_colony_rec->ai_plan_flags & 0x80)
        goto skip_food;
    f9c = 1;
    if (got5 == 0 && g_cur_colony_rec->stock[GOODS_LUMBER] < 0xa) {
        for (pass = 0; pass < 3; pass++) {
            for (i = 0; (int)g_cur_colony_rec->pop > i && g_prod[GOODS_LUMBER] == 0; i++) {
                if (assigned[i] != 0)
                    continue;
                switch (pass) {
                case 0:
                    if (spec[i] != 5)
                        continue;
                    break;
                case 1:
                    if (any_spec(spec[i]))
                        continue;
                    break;
                }
                if (preassign_colonist_to_field(i, 5) == 0) {
                    assigned[i] = 1;
                    got5 = 1;
                    compute_colony_yield();
                }
            }
        }
    }
    if (got5 == 0 && g_cur_colony_rec->stock[GOODS_LUMBER] < 2 && (g_game.turn_counter & 7) == 0) {
        g_cur_colony_rec->stock[GOODS_LUMBER] += 0x64;
        if (g_cur_nation_rec->gold >= 200L)
            g_cur_nation_rec->gold -= 200L;
    }
    lumber_supply = g_cur_colony_rec->stock[GOODS_LUMBER] + g_prod[GOODS_LUMBER];
    if (lumber_supply >= 2) {
        for (pass = 0; pass < 4; pass++) {
            for (i = 0; (int)g_cur_colony_rec->pop > i && g_prod[PROD_HAMMERS] == 0; i++) {
                if (assigned[i] != 0)
                    continue;
                switch (pass) {
                case 0:
                    if (spec[i] != 0xd)
                        continue;
                    break;
                case 1:
                    if (spec[i] != 0x1c)
                        continue;
                    break;
                case 2:
                    if (spec[i] != 0x19)
                        continue;
                    break;
                }
                if (spec[i] == 0x1a || spec[i] == 0x19)
                    set_specialty(i, JOB_COLONIST_ALT);
                if (any_spec(spec[i]) == 0 && list[13] == 0
                    && (int)g_cur_colony_rec->pop >= 6
                    && rand_range(0, 0x10 - g_game.difficulty) == 0)
                    set_specialty(i, JOB_CARPENTER);
                set_job(i, 0xd);
                count[13]++;
                assigned[i] = 1;
                compute_colony_yield();
            }
        }
    }
skip_food:
    for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
        if (assigned[i] != 0)
            continue;
        if (any_spec(spec[i]) == 0 || spec[i] >= 9)
            continue;
        if (spec[i] == 0 || spec[i] == 8)
            continue;
        if (spec[i] == 5) {
            if (f9c == 0)
                f9c = 1;
            else if (!(g_cur_colony_rec->ai_flags & 0x20) && g_shortfall[GOODS_LUMBER] == 0)
                continue;
        }
        if (g_cur_colony_rec->stock[spec[i]] > cap)
            continue;
        if (preassign_colonist_to_field(i, spec[i]) == 0) {
            assigned[i] = 1;
            compute_colony_yield();
        }
    }
    for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
        if (assigned[i] != 0)
            continue;
        if (any_spec(spec[i]) == 0 || spec[i] < 9 || spec[i] >= 0x13)
            continue;
        if (verdict[spec[i]] == 0)
            continue;
        if (count[spec[i]] >= 3)
            continue;
        want = 0;
        switch (spec[i]) {
        case 9:
        case 0xa:
        case 0xb:
        case 0xc:
        case 0xe:
        case 0xf:
            quota = g_goods_input[spec[i]];
            if (g_shortfall[quota] == 0
                && (g_cur_colony_rec->stock[quota] > 0 || g_prod[quota] != 0))
                want = 1;
            break;
        case 0x10:
            want = !(g_game.game_flags & GAMEFLAG_INDEPENDENCE);
            if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
                spec[i] = 0x1c;
                set_specialty(i, JOB_COLONIST_ALT);
                list[16]--;
                continue;
            }
            break;
        case 0x11:
            want = !(g_game.game_flags & GAMEFLAG_INDEPENDENCE);
            break;
        case 0xd:
            if ((g_cur_colony_rec->ai_plan_flags & 0x80) && list[13] > 1) {
                spec[i] = 0x1c;
                set_specialty(i, JOB_COLONIST_ALT);
                list[13]--;
                continue;
            }
            if (g_shortfall[GOODS_LUMBER] == 0
                && (unsigned)(g_cur_colony_rec->stock[GOODS_LUMBER] + g_prod[GOODS_LUMBER]) != 0)
                want = 1;
            break;
        }
        if (want != 0) {
            set_job(i, spec[i]);
            count[spec[i]]++;
            assigned[i] = 1;
            compute_colony_yield();
        }
    }
    for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
        if (assigned[i] != 0)
            continue;
        preassign_colonist_to_field(i, -2);
        best = 0;
        job = 0xd;
        for (k = 9; k < 0x13; k++) {
            if (verdict[k] == 0)
                continue;
            if (k == 0x12)
                continue;
            if (count[k] >= 3)
                continue;
            set_job(i, k);
            good = g_paired_job[k];
            if (k == 0xf)
                good = 0xe;
            if (k == 0xd)
                good = 5;
            if (good >= 0) {
                need = g_prod[good] + g_cur_colony_rec->stock[good] - g_consumption[good];
                if (need < 0)
                    continue;
                if (need == 0)
                    need++;
            }
            val = compute_colonist_yield(i, &bestidx);
            val = min_int_1018_b329(val, need);
            if (bestidx >= 0x10) {
                w = 3;
                if (k == 0x11) {
                    w += tories;
                    w += 4;
                    w += building_level(BUILDING_PRINTING_PRESS) << 2;
                    w += g_cur_colony_rec->capitols << 2;
                    if (tories >= 0xa)
                        w += w;
                    if (has_father(g_cur_colony_rec->nation, FATHER_THOMAS_JEFFERSON))
                        w = w * 2;
                    if (g_game.year < 0x604)
                        w = 0;
                    if (g_game.year > 0x640)
                        w = w * 2;
                    if (g_game.year > 0x6a4)
                        w = w * 2;
                    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
                        w = 0;
                    if ((int)g_cur_colony_rec->pop <= 3)
                        w >>= 1;
                    if ((int)g_cur_colony_rec->pop < 6)
                        w >>= 1;
                    if (g_rank[nation] > g_rank[g_game.player_nation])
                        w >>= 1;
                    if (g_rank[nation] < g_rank[g_game.player_nation])
                        w += w;
                    if (g_cur_nation_rec->flags & 4)
                        w >>= 1;
                    w = w - g_prod[bestidx];
                    w = minimax(w, 1, 0x64);
                }
                if (k == 0xd) {
                    w = 5;
                    w -= (unsigned)g_prod[bestidx] / 3;
                    if (g_cur_colony_rec->ai_plan_flags & 0x80)
                        w = w >> 1;
                    w = max_int_1018_b34f(1, w);
                }
                if (k == 0x10) {
                    w += 6;
                    w -= g_game.turn_counter / 0x64;
                    w -= (unsigned)g_prod[bestidx] >> 1;
                    w = max_int_1018_b34f(1, w);
                }
            } else {
                w = g_goods_value[g_cur_colony_rec->nation][bestidx];
                if (bestidx != 0xf && bestidx != 0xe)
                    w -= (&g_goods_value[0][-8])[g_cur_colony_rec->nation * 16 + bestidx];
                if (bestidx == 0xe || bestidx == 0xf) {
                    w += 4;
                    if (g_game.turn_counter >= 0x32
                        && g_rank[nation] >= g_rank[g_game.player_nation])
                        w += w;
                }
            }
            val = ((val << 3) + 5) * w;
            if (val > best) {
                best = val;
                job = k;
            }
        }
        set_job(i, 0x12);
        if (g_best_job_score >= best) {
            preassign_colonist_to_field(i, -1);
            if (g_best_job_amount == 0) {
                if (!has_building(BUILDING_CHURCH)
                    || !cargo_free(5) || count[16] >= 3)
                    job = 0xd;
                else
                    job = 0x10;
                set_job(i, job);
                count[job]++;
            }
        } else {
            set_job(i, job);
            count[job]++;
        }
        compute_colony_yield();
    }
    farmers = job_count(0) + job_count(8);
    farm_experts = specialty_count(0)
                   + specialty_count(8);
    need_food_bld = 0;
    g_cur_colony_rec->ai_plan_flags &= 0x7f;
    g_cur_colony_rec->item = -1;
    experts = 0;
    maxrank = 0;
    for (i = 0; (int)g_cur_colony_rec->pop + g_tile_colonists > i; i++) {
        if ((int)g_cur_colony_rec->pop > i)
            spc = spec[i];
        else
            spc = get_specialty(i);
        if (any_spec(spc) == 0)
            continue;
        experts++;
        maxrank = max_int_1018_b34f(maxrank, g_spec_recs[spc].school % 4);
    }
    if (((int)g_cur_colony_rec->pop >> 1) < farmers && farmers > 1)
        need_food_bld = 1;
    if (g_shortfall[GOODS_FOOD] != 0)
        need_food_bld = 1;
    if ((int)g_cur_colony_rec->pop >= tiles - unworkable
        || (unworkable != 0 && need_food_bld != 0)) {
        if (try_to_build(BUILDING_DOCKS) == 0) {
            need_food_bld = 0;
            goto done_build;
        }
    }
    if (try_to_build(BUILDING_STOCKADE) == 0)
        goto done_build;
    if (g_cur_colony_rec->stock[GOODS_HORSES] >= 2 && try_to_build(BUILDING_STABLES) == 0)
        goto done_build;
    if ((int)g_cur_colony_rec->pop < 4) {
        g_cur_colony_rec->ai_plan_flags |= 0x80;
        goto done_build;
    }
    want_warehouses = (int)g_cur_colony_rec->pop / 6;
    if (g_cur_colony_rec->warehouses < (unsigned char)want_warehouses && g_cur_colony_rec->warehouses < 1
        && try_to_build(BUILDING_WAREHOUSE_EXPANSIONS) == 0)
        goto done_build;
    if ((int)g_cur_colony_rec->pop >= 6
        && ((g_cur_colony_rec->ai_flags & (COLONY_AI_BESIEGED | COLONY_AI_BLOCKADED))
            || g_nation_armed_ships[g_game.player_nation] - 2 > g_nation_armed_ships[nation]
            || (int)g_cur_colony_rec->pop >= 0xc)) {
        if (try_to_build(BUILDING_CUSTOM_HOUSE) == 0)
            goto done_build;
    }
    if (!(g_cur_colony_rec->status & 0x20) && g_game.year < 0x640
        && (g_region_flags[region] & REGION_NATIVE_SETTLEMENT)
        && pissed(g_cur_tribe, nation) < TRIBE_ATTITUDE_BAD && adj_threat == 0) {
        g_cur_colony_rec->item = build_me_a(0xc);
        goto done_build;
    }
    if (maxrank >= 1 && (int)g_cur_colony_rec->pop + experts >= 4
        && try_to_build(BUILDING_SCHOOLHOUSE) == 0)
        goto done_build;
    if ((int)g_cur_colony_rec->pop < 6)
        g_cur_colony_rec->ai_plan_flags |= 0x80;
    if (((g_goods_value[nation][0xf] + (g_game.difficulty >> 1) >= 4 || g_game.turn_counter > 0x50)
         && (int)g_cur_colony_rec->pop >= 6
         && (g_cur_colony_rec->stock[GOODS_TOOLS] >= 0x28 || g_prod[GOODS_TOOLS] != 0))
        || list[15] != 0) {
        if (try_to_build(BUILDING_ARMORY) == 0)
            goto done_build;
    }
    if (list[16] != 0 && try_to_build(BUILDING_CHURCH) == 0)
        goto done_build;
    if (try_to_build(BUILDING_LUMBER_MILL) == 0)
        goto done_build;
    if (try_to_build(BUILDING_FORT) == 0)
        goto done_build;
    if (g_goods_value[nation][0xf] >= 4 && (int)g_cur_colony_rec->pop >= 4
        && (g_cur_colony_rec->stock[GOODS_ORE] >= 0x28 || g_prod[GOODS_ORE] != 0)) {
        if (try_to_build(BUILDING_BLACKSMITHS_SHOP) == 0)
            goto done_build;
    }
    if (g_cur_colony_rec->warehouses < (unsigned char)want_warehouses
        && try_to_build(BUILDING_WAREHOUSE_EXPANSIONS) == 0)
        goto done_build;
    if ((unsigned)g_prod[PROD_BELLS] >= 0x18 && try_to_build(BUILDING_NEWSPAPER) == 0)
        goto done_build;
    if ((unsigned)g_prod[PROD_BELLS] >= 4 && try_to_build(BUILDING_NEWSPAPER) == 0)
        goto done_build;
    if (maxrank >= 2 && (int)g_cur_colony_rec->pop + experts >= 0xa
        && try_to_build(BUILDING_COLLEGE) == 0)
        goto done_build;
    if ((int)g_cur_colony_rec->pop < 8) {
        g_cur_colony_rec->ai_plan_flags |= 0x80;
        goto done_build;
    }
    if (maxrank >= 3 && (int)g_cur_colony_rec->pop + experts >= 0x10
        && try_to_build(BUILDING_UNIVERSITY) == 0)
        goto done_build;
    if (try_to_build(BUILDING_CHURCH) == 0)
        goto done_build;
    if ((int)g_cur_colony_rec->pop >= 0xa && try_to_build(BUILDING_FORTRESS) == 0)
        goto done_build;
    have3 = 0;
    for (i = 5; i >= 0; i--) {
        if (building_level(g_build_needs[i].building) == 3)
            have3 = 1;
    }
    if (have3 != 0 && (g_cur_colony_rec->status & 0x40)) {
        if (try_to_build(BUILDING_SHIPYARD) == 0)
            goto done_build;
        if (has_building(BUILDING_SHIPYARD)) {
            if ((g_nation_population[nation] >> 1) + g_nation_colony_count[nation]
                >= g_nation_cargo_capacity[nation]) {
                g_cur_colony_rec->item = build_me_a(0xf);
                goto done_build;
            }
            if ((g_unit_tally[nation][UNIT_FRIGATE] > 0 || g_nation_armed_ships[nation] == 0)
                && g_nation_armed_ships[nation] < 4) {
                g_cur_colony_rec->item = build_me_a(0x10);
                goto done_build;
            }
            if (g_unit_tally[nation][UNIT_FRIGATE] < 1) {
                g_cur_colony_rec->item = build_me_a(0x11);
                goto done_build;
            }
        }
    }
    artillery = 0;
    for (u = top_unit_in_square(x, y); u >= 0; u = next_unit(u)) {
        if (g_units[u].type == UNIT_ARTILLERY)
            artillery++;
    }
    if (have3 != 0 && (artillery == 0 || soldiers == 0)) {
        if (g_cur_colony_rec->stock[GOODS_TOOLS] != 0
            && try_to_build(BUILDING_ARMORY) == 0)
            goto done_build;
        g_cur_colony_rec->item = build_me_a(0xb);
        goto done_build;
    }
    for (i = 5; i >= 0; i--) {
        if (needs_a_building(i)) {
            if (try_to_build(
                    top_building(g_build_needs[i].building)) == 0)
                goto done_build;
        }
    }
    if (try_to_build(BUILDING_CATHEDRAL) == 0)
        goto done_build;
    if (has_building(BUILDING_SHIPYARD)) {
        if (min_int_1018_b329(8, g_nation_armed_ships[nation]) > g_unit_tally[nation][UNIT_GALLEON]) {
            g_cur_colony_rec->item = build_me_a(0xf);
            goto done_build;
        }
        if (g_nation_armed_ships[nation] < 8) {
            g_cur_colony_rec->item = build_me_a(0x11);
            goto done_build;
        }
    }
    if ((int)g_cur_colony_rec->pop < 0xa)
        g_cur_colony_rec->status |= COLONY_AI_SMALL;
    if (artillery < 3) {
        if (try_to_build(BUILDING_ARMORY) == 0)
            goto done_build;
        if (g_prod[GOODS_MUSKETS] != 0 && try_to_build(BUILDING_ARSENAL) == 0)
            goto done_build;
        g_cur_colony_rec->item = build_me_a(0xb);
        goto done_build;
    }
    g_cur_colony_rec->item = -1;
    g_cur_colony_rec->ai_plan_flags |= 0x80;
done_build:
    school_level = building_level(BUILDING_SCHOOLHOUSE);
    limit2 = ((school_level == 3) + school_level) << 2;
    if (school_level != 0 && (char)g_cur_colony_rec->ai_idle_turns >= (char)limit2) {
        newspec = -1;
        if ((int)g_cur_colony_rec->pop < 0xa && farm_experts <= farmers) {
            if (specialty_count(8) < water)
                newspec = 8;
            else
                newspec = 0;
        }
        for (i = 0; i < 6; i++) {
            if (building_level(g_build_needs[i].building)
                < (g_build_needs[i].building == 3 ? 1 : 2))
                continue;
            if (g_prod[g_build_needs[i].goods] == 0)
                continue;
            if (list[g_build_needs[i].expert_job] != 0)
                continue;
            newspec = i;
        }
        nlist = 0;
        for (i = 0; (int)g_cur_colony_rec->pop + g_tile_colonists > i; i++) {
            job = get_job(i);
            spc = get_specialty(i);
            if (any_spec(spc) == 0
                || (spc != job && job != JOB_COLONIST)) {
                if (spc != 0x1b && nlist < 0x19) {
                    list[nlist] = i;
                    nlist++;
                }
            }
        }
        if (nlist != 0) {
            pick2 = rand_range(0, nlist - 1);
            pick = list[pick2];
            job = newspec >= 0 ? newspec : get_job(pick);
            set_specialty(pick, job);
            g_cur_colony_rec->ai_idle_turns = 0;
        }
    }
    if (need_food_bld != 0 && (int)g_cur_nation_rec->tax <= 0x19
        && g_spec_recs[0].price <= g_cur_nation_rec->gold) {
        experts = 0;
        pick = -1;
        for (i = 0; (int)g_cur_colony_rec->pop > i; i++) {
            job = get_specialty(i);
            if (job == JOB_FARMER)
                experts |= 1;
            if (job == JOB_FISHERMAN)
                experts |= 2;
            if (any_spec(job) == 0 && job != JOB_CONVERT)
                pick = i;
        }
        job = 0x1c;
        if (pick >= 0) {
            if (!(experts & 2) && has_building(BUILDING_DOCKS))
                job = 8;
            else if (!(experts & 1))
                job = 0;
        }
        if (job != JOB_COLONIST_ALT) {
            set_specialty(pick, job);
            g_cur_nation_rec->gold -= g_spec_recs[pick].price;
            raise_taxes("KINGTAX", 1);
        }
    }
    if (g_cur_colony_rec->stock[GOODS_HORSES] < 2 && g_game.turn_counter >= 0x28) {
        u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
        if (g_unit_types[g_units[u].type].holds > 0 && g_cur_nation_rec->gold >= 10L) {
            g_cur_nation_rec->gold -= 10L;
            g_cur_colony_rec->stock[GOODS_HORSES] = 2;
        }
    }
    g_ai_managing_colony = 0;
}
