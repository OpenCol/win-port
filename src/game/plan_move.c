/* PORT: this file's own declarations of colony_add_unit_2 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of build_another_colony, combat_fight, find_my_close_unit, passenger, unit_build_another, unit_build_factor removed: the header declares them from the definition (W1 step 3). */
/* plan_move: candidate for 1028:5329 (19764 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: plan_move
 * name-source: sibling-build
 *
 * ONE COMPUTER-CONTROLLED UNIT'S WHOLE TURN -- the largest function in the binary. It is called
 * once per unit per turn and it either gives the unit a mission and a destination, moves it one
 * square, attacks with it, loads it, disbands it, or leaves it alone. The unit's own mission byte
 * (Unit+7) and its orders byte (Unit+8) are the state it carries between turns; this function is
 * the only thing that writes the mission byte for a computer nation.
 *
 * IT RETURNS 1 when the unit was absorbed into a colony (`colony_add_unit_2`) or its record
 * was freed (`delete_unit`, then `goto retlbl` past the `ok = 0`), and 0 in every other case -- the caller uses that to know
 * whether the unit index it holds is still the same unit.
 *
 * WHAT IT DECIDES, in order:
 *
 *   1. NOTHING TO DO. A unit whose orders are 1..4, 7, 8 or 9 -- sentry, trade route, build
 *      colony, plow, road -- is already busy and returns at once. Off the map, the mission is set
 *      to 0x40 and it returns.
 *   2. THE SURVEY. It measures where the unit is against everything that matters: the nearest
 *      colony of anybody, of its own nation, and of its own nation on another continent; the
 *      nearest native settlement, that tribe's attitude quartered, and what that settlement
 *      thinks of this nation; the terrain, the road and river bits, the land region and this
 *      nation's state in it (SEG24:0x500), and the tile's improvement decoration.
 *   3. IS THERE WORK. `wantwork` starts as "this is a colonist or a pioneer", is cleared for an
 *      expert of profession 0x1b, and is then argued over by the soldier rules (orders 0 or 0xb
 *      from home, a weak region, no adjacent enemy), the scout rules (mission 0x32, an
 *      unexplored region every fifteenth turn, more than twelve from home), the trade-route
 *      total, and finally the per-region visit counter at SEG24:0x6d6, which allows two colonists
 *      or three of anything else per region per turn.
 *   4. THE COLONY SITE. With work to do, every square within a radius that shrinks from three to
 *      one as the unit's idle counter grows is scored: the plane-3 nibble times four, minus the
 *      square of (seven minus the distance) to the nearest foreign colony inside seven (nine for one of ours),
 *      minus a native-anger term that is eight times the tribe's attitude-and-level figure scaled
 *      by distance and halved again for the French, for a nation with father 0x10, and quartered
 *      for the Spanish. The winner gets mission 0x32, or orders 7 -- BUILD COLONY -- if the unit is
 *      already standing on it.
 *   5. JOINING A COLONY. A colonist with no work walks to the colony of its own nation whose
 *      (min(16, max size) - population) times half the distance is smallest (doubled when full) and which still has room, and
 *      is absorbed there. With no such colony and standing in one, it turns into a PIONEER
 *      (type 2, 20 tools); otherwise it goes back to step 4 with work forced on.
 *   6. LOADING AND UNLOADING. A carrier standing in one of its own colonies drops its whole cargo
 *      into the colony store, wakes every sentried unit aboard, takes aboard the soldiers and
 *      pioneers the colony can spare, and then fills the remaining holds with the goods whose
 *      per-nation value times amount is highest -- on a ship by value, on a wagon by scarcity.
 *   7. WHERE THE CARGO GOES. A loaded ship scores every other colony of its nation by what the
 *      cargo is worth there, by the colony's own shortage, and by distance over four; a ship with
 *      soldiers or pioneers aboard instead scores the eight land squares around it and unloads
 *      onto the one whose region wants them. With nothing worth carrying it heads for the sea lane.
 *   8. THE SPECIAL UNITS. A wagon train (type 0xc) drives to the nearest native settlement, or to
 *      its home colony, or is disbanded. A treasure train (type 0xa) standing in a colony is
 *      cashed at a hundred gold per unit of its `spec` byte and disbanded with the LOOTFOREIGN
 *      message; elsewhere it walks to a galleon or to a colony. A missionary (type 3) picks the
 *      settlement where attitude over distance is highest and takes mission 0x4a, or becomes a
 *      plain colonist.
 *   9. THE MOVE. Failing all of that, the eight neighbours and staying put are scored -- terrain
 *      value, road and river bonuses, plane-3 exploration value, the odds `combat_fight` gives
 *      against whoever is there scaled by the defenders and by the unit's own attack rating, a
 *      penalty for turning away from the direction the unit was already going, and, for a ship,
 *      the value of the unexplored squares four away. The best direction becomes Unit+0xb and the
 *      unit is given orders 0xc with that destination.
 *
 * FIVE SHIPPED BUGS, all reproduced because the bytes require them:
 *
 *   - THE CENTRE-SQUARE BONUS IS TESTED AGAINST THE MAP CORNER. Step 4 adds sixteen to a square's
 *     score when `tx == 0 && ty == 0` -- absolute map coordinates, not the unit's own square. The
 *     bonus therefore only ever applies to the top-left corner of the map, which is ocean.
 *   - ONE SQUARE TEST READS AN UNINITIALISED LOCAL. When `water_access` says the
 *     square has no coast, the score is zeroed only `if (dd2 == 8)`, and `dd2` at that point is
 *     the direction counter of a loop that has not run yet.
 *   - THE CARGO AMOUNT IS COMPUTED TWICE. Step 6 works out how much of the chosen goods to load,
 *     adjusts it for food, for horses and for the two war goods, clamps it at zero -- and then
 *     recomputes it as `min(100, colony stock)`, throwing every adjustment away.
 *   - THE MISSIONARY'S INFLUENCE TERM IS DEAD. `village_anger` is called and its
 *     cell count added to the score, and the next statement assigns over the score with the
 *     attitude figure. The call is kept only for its side effect on the current settlement.
 *   - A SHIP'S LAND-SQUARE TEST IS DEAD. In step 9 a ship scoring a land (non-ocean) square runs three
 *     tests whose every outcome is `continue`, so the type-0x11 (frigate) test can never decide anything.
 *
 * WHAT THE BYTES SETTLED. Forty-one divergences were fixed to get here; six of them taught
 * something the corpus did not know:
 *
 *   - A `(unsigned char)` CAST OVER AN EXPRESSION CONTAINING A REGISTER CANDIDATE COSTS IT ITS
 *     REGISTER. `!(unsigned char)((g_game.turn_counter + u) & 0x1f)` compiles correctly but silently pushes `u`
 *     out of SI and shifts every frame slot below it by two. Written as `((g_game.turn_counter + u) & 0x1f) == 0`
 *     it keeps the register AND still emits the byte `TEST AL,0x1f`. This is why the frame came out
 *     two bytes short for a long time, and it is worth knowing before the next big function.
 *   - `u` IS A REGISTER LOCAL, NOT A REGISTER PARAMETER. `int f(register int u)` is refused;
 *     `int f(int unit) { register int u; ... u = unit; }` is what produces `MOV SI,[BP+6]` as
 *     the first instruction.
 *   - `(v + k) * 2` DISTRIBUTES into `v*2 + 2k`; `(v + k) << 1` does not. The shipped has
 *     `ADD AX,3 / ADD AX,AX`, which only the shift spelling produces.
 *   - A COMPOUND `*=` BY A CONSTANT loads the constant into AX and uses the one-operand `IMUL mem`;
 *     the plain `v = k * v` uses the 386 three-operand `IMUL AX,AX,k`.
 *   - `(unsigned char)(f() - 1)` GIVES `ADD AL,0xFF` where a plain `f() - 1` gives `DEC AL`.
 *   - `!(b >> 4)` TESTS THE WHOLE WORD (`OR AX,AX`); `(b >> 4) == 0` narrows to `OR AL,AL`.
 *
 * Also: `EB 00` twice over (rule 54) is two empty `else` branches, one on the "somebody is here"
 * test and one on the "it is not us" test; `rand_range` returns `int` in this module, not the
 * `long` the corpus usually declares (rule 9); and the frame is 108 word scalars, six compiler
 * temporaries for the three `max_ptr_1028_e5ff` calls, and `char cargo[16]` last. The calls
 * were by reference; the module's one max_ptr takes pointers, so each temporary is named
 * (t1..t6, second argument's first) and passed as `(t = v, &t)`, which keeps their slots.
 *
 * UNIT+3 VERSUS UNIT+4, corrected 2026-09-20 by tools/check_global_addresses.py. Six
 * uses of `.flags` read the member at +3 while the shipped code reads +4. The three that
 * are right are the ones testing UNIT_NATION_MASK and `>> 4` -- that is Unit+3, the
 * nation in the low nibble and who has spotted the unit in the high one. The six that
 * were wrong all test single bits (0x20, 0x10, 0x2), which is a different flag byte at
 * +4, here `f4`. A member offset is folded into the masked displacement FIXUP, so every
 * one of them compiled to the shipped bytes and the file graded RELOCATABLE.
 *
 * RENAMED 2026-09-26 from ai_unit_turn: the Mac build's plan_move(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (callee add_strategy; callee at_plan; callee kill_strategy; callee on_map; callee terrain_at), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "fathers.h"
#include "goods.h"
#include "control.h"
#include "royal.h"
#include "aicol.h"
#include "settle.h"
#include "gameflag.h"
#include "nations.h"
#include "tribes.h"
#include "terrain.h"
#include "jobs.h"
#include "units.h"
#include "region.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern Pickup g_cargo_pickups[];                          /* SEG24:0x676, 6 bytes each */

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern int g_units_expelled;                       /* DGROUP:0x1182, declared */

extern int zoc(int x, int y, int n);
extern int find_close_colony(int x, int y, int nation, int land);
extern int land_region_at(int x, int y);
extern int find_close_village(int x, int y, int nation, int land);
extern int pissed(int tribe, int n);
extern int piss_factor(int tension);

extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);
extern int resource_at(int x, int y);
extern int map_dist(int x, int y, int x2, int y2);
extern void set_colony(int colony);
extern int zoc_4(int x, int y, int nation);
extern int top_unit(int u);
extern int next_unit(int u);
extern int which_move(int dx, int dy);
extern int is_colonist(int u);
extern int is_anything(int x, int y);
extern unsigned char site_get(int x, int y);
extern int water_access(int x, int y);
extern unsigned char continent_at(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int has_father(int nation, int bit);
extern void set_village(int v);
extern int colony_max_size(void);
extern int stack_check(int u, int kind);
extern void end_unit_turn(int u);
extern void set_europe(int n);
extern int unload_cargo(int u, int slot);
extern int get_warehouse(void);

extern int stow_cargo(int u, int g, int amount);

extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern void stack_ship(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern int terrain_is_ocean(int x, int y);
extern void move_a_unit(int u, int dir);
extern void set_course_for_europe(int u);
extern void traffic_to_europe(int g, int n);
extern void delete_unit(int u);

extern void popup_say(int slot, int t);
extern int popm(char *msg, int k);
extern int get_treaty(int nation_a, int nation_b);
extern int village_anger(int v, int *strength);
extern int tribe_radius(int tribe);
extern char owner_of(int x, int y);
extern int is_colony(int x, int y);
extern int is_village(int x, int y);
extern int is_unit(int x, int y);

extern int colony_has_building(int colony, int bit);
extern unsigned char move_rate(int u);
extern int bottom_unit(int u);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int menaced(int x, int y, int nation, int flag);
extern "C" int find_beach(int nation, int x, int y, int use_status, int artillery);
extern "C" void add_strategy(int nation, int x, int y, int kind, int priority);
extern "C" void kill_strategy(int nation, int kind, int x, int y, int rad);


extern "C" int region_build_factor(int nation, int region);
extern "C" int at_plan(int nation, int x, int y, int kind);
extern "C" void order_to(int u, int m, int x, int y);

#define ABS(v) ((v) > 0 ? (v) : ~(v) + 1)

extern "C" int plan_move(int unit)
{
    register int u;
    register int d;
    int ok;
    int nation;
    int hn;
    int x;
    int y;
    int nx;
    int ny;
    int tx;
    int ty;
    int nx2;
    int ny2;
    int dir;
    int type;
    int claim;
    int anycol;
    int anycoldist;
    int anycolregion;
    int mycol;
    int mycoldist;
    int mycolregion;
    int vill;
    int villdist;
    int villregion;
    int attq;
    int vill_anger;
    int col2;
    int col2dist;
    int terrain;
    int terrain2;
    int region;
    int vregion;
    int deco;
    int regstate;
    int tm1;
    int dd2;
    int dd;
    int best;
    int bestp3;
    int bestx;
    int besty;
    int plane3;
    int occupant;
    int col;
    int delta;
    int iswater;
    int river;
    int improved;
    int good;
    int nland;
    int n3;
    int n4;
    int n6;
    int n5;
    int n3copy;
    int flags;
    int tot;
    int t74;
    int saveu;
    int attacked;
    int v7a;
    int retflag;
    int anymoved;
    int bestcol;
    int ci;
    int ringt;
    int freeflag;
    int defenders;
    int base;
    int score;
    int cap;
    int amount;
    int take;
    int routetotal;
    int wantwork;
    int band;
    int lvl;
    int ctype;
    int amt;
    int isship;
    int dist;
    int score2;
    int maxtype;
    int cap2;
    int adjocean;
    int limit;
    int attackdir;
    int res;
    int hf;
    int mul;
    int aggr;
    int aggr2;
    int mine;
    int art;
    int idle;
    int tblmax;
    int moved;
    int capital;
    int radius;
    int ismission;
    int pen;
    int infl;
    int cell;
    int tm7;
    int okflag;
    int uu;
    int bonus;
    int amountl;
    int v_da;
    int t1, t2, t3, t4, t5, t6;
    char cargo[16];

    u = unit;
    ok = 1;
    retflag = 0;
    idle = 0;
    v_da = 0;
    nation = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
    if (g_units[u].orders != ORDERS_NONE && g_units[u].orders != ORDERS_FORTIFY && g_units[u].orders != ORDERS_FORTIFIED
        && g_units[u].orders < 0xa)
        goto out;
    x = g_units[u].x;
    y = g_units[u].y;
    dir = 8;
    type = g_units[u].type;
    if (on_map(x, y) == 0) {
        g_units[u].mission = 0x40;
        goto out;
    }
    ismission = ((int)g_units[u].mission == 0x74 || (int)g_units[u].mission == 0x69);
    claim = zoc(x, y, nation);
    anycol = find_close_colony(x, y, -1, -1);
    anycoldist = g_nearest_dist;
    anycolregion = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    mycol = find_close_colony(x, y, nation, -1);
    mycoldist = g_nearest_dist;
    if (mycol >= 0)
        mycolregion = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    else
        mycolregion = -2;
    vill = find_close_village(x, y, -1, -1);
    villdist = g_nearest_dist;
    villregion = land_region_at(g_cur_settlement_rec->x, g_cur_settlement_rec->y);
    attq = piss_factor(pissed(g_cur_tribe, nation));
    vill_anger = g_cur_settlement_rec->anger[nation];
    col2 = find_close_colony(x, y, nation, -2);
    col2dist = g_nearest_dist;
    isship = (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST);
    terrain = terrain_at(x, y);
    iswater = (terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE);
    river = map_get(x, y) & MAP0_RIVER;
    improved = feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD);
    region = land_region_at(x, y);
    if (region >= 0)
        regstate = g_region_state[nation][region];
    else
        regstate = 5;
    deco = resource_at(x, y);
    routetotal = unit_build_another(nation, u);
    wantwork = (g_units[u].type == UNIT_PIONEERS || g_units[u].type == UNIT_COLONISTS);
    if ((int)g_units[u].spec == JOB_CONVERT)
        wantwork = 0;
    if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS) {
        if (g_units[u].orders == ORDERS_NONE)
            wantwork = 1;
        if (g_units[u].orders == ORDERS_MISSION) {
            if (map_dist(x, y, g_units[u].dest_x, g_units[u].dest_y) > 0xc)
                wantwork = 1;
        }
        if (region_build_factor(nation, region) > 2)
            wantwork = 1;
        if (menaced(x, y, nation, 0) >= 0)
            wantwork = 0;
        if ((int)g_units[u].spec == JOB_SOLDIER)
            wantwork = 0;
        if (g_region_colony_count[nation][region] == 0 && g_region_attack[nation][region] < 8)
            wantwork = 1;
        if (g_units[u].type == UNIT_DRAGOONS && (g_region_flags[region] & REGION_RIVAL_COLONY))
            wantwork = 0;
    }
    if (g_units[u].type == UNIT_SCOUTS) {
        if ((int)g_units[u].mission == 0x32)
            wantwork = 1;
        if (regstate == 0)
            wantwork = 0;
        if (g_region_colony_count[nation][region] == 0 && g_game.turn_counter % 0xf == 0)
            wantwork = 1;
        if (mycoldist > 0xc && anycoldist > 2)
            wantwork = 1;
        if (menaced(x, y, nation, 0) >= 0 || g_game.year > 0x672)
            wantwork = 0;
    }
    if (isship == 0 && g_unit_types[g_units[u].type].attack > 1
        && (int)g_units[u].home < 0 && mycoldist <= 8 && mycolregion == region)
        g_units[u].home = mycol;
    wantwork = (wantwork != 0 && routetotal != 0);
    if (g_units[u].type == UNIT_COLONISTS && mycoldist == 0) {
        set_colony(mycol);
        if (g_cur_colony_rec->ai_flags & COLONY_AI_HAS_ROOM)
            wantwork = 0;
    }
    if (wantwork != 0 && region >= 0) {
        if (++g_region_unit_tally[region] > (g_units[u].type == UNIT_COLONISTS ? 2 : 3))
            wantwork = 0;
    }
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        wantwork = 0;
    aggr = 1;
    if (g_units[u].type == UNIT_MAN_O_WAR)
        aggr = 0;
    if (g_units[u].type == UNIT_FRIGATE) {
        good = g_nation_cargo_capacity[nation];
        good -= g_unit_tally[nation][g_units[u].type] * 3;
        good -= g_unit_tally[nation][UNIT_PRIVATEER];
        if (good >= 4)
            aggr = 0;
        if (zoc_4(x, y, nation) != 0)
            aggr = 0;
    }
    if (g_units[u].type == UNIT_PRIVATEER)
        aggr = (g_colonies_blockaded > 1 || g_pop_blockaded > 6);
    aggr2 = aggr;
    if (g_units[u].type == UNIT_MAN_O_WAR) {
        if ((u & 1) != 0 || g_unit_tally[nation][UNIT_MAN_O_WAR] == 1)
            aggr2 = 1;
    }
    if (isship == 0 && g_unit_types[g_units[u].type].attack > 1
        && g_units[u].type != UNIT_DRAGOONS && g_units[u].type != UNIT_CAVALRY && mycoldist == 0) {
        set_colony(mycol);
        if ((int)g_cur_colony_rec->want_defenders > 0 || (int)g_units[u].mission == 0x41) {
            defenders = 0;
            saveu = u;
            for (u = top_unit(u); u >= 0; u = next_unit(u)) {
                if (g_unit_types[g_units[u].type].attack > 1
                    && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                    && g_units[u].type != UNIT_DRAGOONS && g_units[u].type != UNIT_CAVALRY)
                    defenders++;
            }
            u = saveu;
            if ((int)g_units[u].mission != 0x41) {
                if (defenders > 1) {
                    retflag = 1;
                    goto after_all;
                }
                g_cur_colony_rec->want_defenders--;
                g_units[u].mission = 0x47;
            }
            dir = 8;
            goto move;
        }
    }
    if (regstate == 0 && ismission == 0 && isship == 0
        && (g_units[u].type == UNIT_PIONEERS || g_unit_types[g_units[u].type].attack > 1)
        && mycolregion == region) {
        if (mycoldist != 0) {
            set_colony(mycol);
            order_to(u, 0x56, g_cur_colony_rec->x, g_cur_colony_rec->y);
            goto out;
        } else {
            g_units[u].mission = 0x56;
            dir = 8;
            goto move;
        }
    }
    if (g_units[u].type == UNIT_SCOUTS && villdist == 1 && !(g_cur_settlement_rec->flags & SETTLEMENT_CHIEF_SPOKE)
        && attq < 0x19 && vill_anger == 0) {
        dir = which_move(g_cur_settlement_rec->x - x,
            g_cur_settlement_rec->y - y);
        g_units[u].mission = 0x4c;
        goto move;
    }
    if (is_colonist(u) && g_unit_types[g_units[u].type].attack <= 1
        && g_units[u].type != UNIT_SCOUTS && g_units[u].type != UNIT_MISSIONARIES
        && ((int)g_units[u].spec == JOB_COLONIST_ALT || (int)g_units[u].spec == JOB_INDENTURED_SERVANT)
        && villdist == 1 && !(g_cur_settlement_rec->flags & SETTLEMENT_TAUGHT)
        && attq < 0x19 && vill_anger < 0x40) {
        dir = which_move(g_cur_settlement_rec->x - x,
            g_cur_settlement_rec->y - y);
        g_units[u].mission = 0x4c;
        goto move;
    }
    tblmax = at_plan(nation, x, y, 6);
    if (wantwork != 0)
        kill_strategy(nation, 6, x, y, 0);
extern "C" retry_site: if (wantwork != 0);
    if (wantwork != 0) {
        dir = 8;
        best = -999;
        bestp3 = 0;
        band = 3;
        if (g_region_farmland[region] <= 8)
            band = 0;
        else if (g_region_farmland[region] <= 0x18)
            band = 1;
        else if (g_region_farmland[region] <= 0x30)
            band = 2;
        radius = 3;
        if (idle >= 0x20)
            radius = 2;
        if (idle >= 0x40)
            radius = 1;
        for (ty = y - radius; y + radius >= ty; ty++) {
            for (tx = x - radius; x + radius >= tx; tx++) {
                if (on_map(tx, ty) == 0)
                    continue;
                occupant = is_anything(tx, ty);
                if (occupant >= 0 && occupant != nation)
                    continue;
                if (land_region_at(tx, ty) != region)
                    continue;
                plane3 = site_get(tx, ty) & 0xf;
                d = plane3 << 2;
                if (tx == 0 && ty == 0)
                    d += 0x10;
                if (terrain_at(tx, ty) == TERRAIN_MOUNTAINS)
                    continue;
                adjocean = water_access(tx, ty);
                if (adjocean != 0) {
                    if (continent_at(g_ocean_x, g_ocean_y) != 1)
                        adjocean = 0;
                }
                if (adjocean == 0) {
                    plane3 = 0;
                    if (dd2 == 8)
                        d = 0;
                    continue;
                }
                find_close_colony(tx, ty, -1, region);
                if (g_cur_colony_idx >= 0) {
                    if (g_nearest_dist <= 1)
                        continue;
                    if (g_cur_colony_rec->nation != (unsigned char)nation) {
                        if (g_nearest_dist == 2)
                            d -= 0x14;
                        limit = 7;
                        if (g_region_colony_count[nation][region] < 1)
                            limit = 5;
                        dist = g_nearest_dist;
                        if (dist < limit)
                            d -= (limit - dist) * (limit - dist);
                    } else {
                        if (g_nearest_dist == 2)
                            continue;
                        limit = 9;
                        dist = g_nearest_dist;
                        if (dist < limit)
                            d -= (limit - dist) * (limit - dist);
                    }
                }
                set_colony(anycol);
                freeflag = 1;
                saveu = u;
                for (dd = 0; freeflag != 0 && dd < 9; dd++) {
                    nx = g_dir_dx[dd] + tx;
                    ny = g_dir_dy[dd] + ty;
                    for (u = top_unit_in_square(nx, ny); u >= 0; u = next_unit(u)) {
                        if (g_units[u].orders == ORDERS_BUILD_COLONY && u != saveu)
                            freeflag = 0;
                    }
                }
                u = saveu;
                if (freeflag == 0)
                    continue;
                find_close_village(tx, ty, -1, -1);
                if (g_cur_settlement >= 0) {
                    vregion = land_region_at(g_cur_settlement_rec->x,
                        g_cur_settlement_rec->y);
                    dist = g_nearest_dist;
                    if (g_region_colony_count[nation][vregion] == 0)
                        dist++;
                    if (dist < 6) {
                        base = (piss_factor(
                            pissed(g_cur_tribe, nation))
                            + g_cur_tribe_rec->level + 3) << 1;
                        if (region != vregion)
                            base = base >> 1;
                        score = 0;
                        score += base >> 1;
                        if (dist <= 4)
                            score += base;
                        if (dist <= 3)
                            score += base * 2;
                        if (dist <= 2)
                            score += base << 2;
                        if (dist <= 1)
                            score += base << 3;
                        if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
                            score = score * 2;
                        if (nation == NATION_FRANCE)
                            score = score >> 1;
                        if (has_father(nation, FATHER_POCAHONTAS))
                            score = score >> 1;
                        if (nation == NATION_SPAIN)
                            score = score >> 2;
                        if (idle > 0x28)
                            score = score >> 1;
                        score -= g_region_attack[nation][vregion];
                        score = max_int_1028_e350(0, score);
                        d -= score;
                    }
                }
                set_village(vill);
                if (wantwork != 0 && plane3 >= 4) {
                    if (g_region_colony_count[nation][region] == 0)
                        d += d >> 1;
                    if (g_units[u].type == UNIT_COLONISTS)
                        d = d * 2;
                    d += idle >> band;
                    if (tblmax != 0)
                        d += 0x10;
                }
                if (d >= best) {
                    best = d;
                    bestp3 = plane3;
                    bestx = tx;
                    besty = ty;
                }
            }
        }
        if (bestp3 > 0) {
            if (wantwork != 0) {
                if (x == bestx && y == besty) {
                    g_units[u].orders = ORDERS_BUILD_COLONY;
                    goto out;
                } else {
                    order_to(u, 0x32, bestx, besty);
                    goto out;
                }
            } else {
                add_strategy(nation, g_dir_dx[dir] + x,
                    g_dir_dy[dir] + y, 6, 2);
            }
        }
    }
extern "C" after_site: if (g_units[u].type == UNIT_COLONISTS && wantwork == 0);
    if (g_unit_types[g_units[u].type].holds > 0 && mycoldist == 0
        && ((g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
        || g_units[u].home == (signed char)mycol)) {
        saveu = u;
        for (u = top_unit(u); u >= 0; u = next_unit(u)) {
            if (g_units[u].orders == ORDERS_SENTRY)
                g_units[u].orders = ORDERS_NONE;
        }
        u = saveu;
        set_colony(mycol);
        set_europe(nation);
        while (g_units[u].cargo_count != 0) {
            good = unload_cargo(u, 0);
            g_cur_colony_rec->stock[good] += g_cargo_amount;
        }
        if (isship != 0)
            g_units[u].home = -1;
        if (isship != 0)
            g_cur_colony_rec->turns_since_ship = 0;
        cap = g_unit_types[g_units[u].type].holds - g_units[u].cargo_count;
        if (g_units[u].type == UNIT_WAGON_TRAIN)
            cap = min_int_1028_e376(1, cap);
        if (isship != 0 && (g_cur_colony_rec->ai_flags & COLONY_AI_BLOCKADED) && g_units[u].type < UNIT_GALLEON) {
            g_units[u].counter++;
            if (g_units[u].counter < 0xa - g_unit_types[g_units[u].type].holds) {
                g_units[u].mission = 0x43;
                dir = 8;
                goto move;
            }
        }
        if (isship != 0 && aggr2 != 0 && !(g_units[u].flags2 & 0x20)) {
            saveu = u;
            for (u = top_unit(u);
                u >= 0 && g_colony_tally[nation] < 0x19;
            u = next_unit(u)) {
                if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                    continue;
                if (g_unit_types[g_units[u].type].size > (unsigned char)cap)
                    continue;
                take = 0;
                if ((g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                    && g_unit_types[g_units[u].type].attack > 1
                    && (int)g_units[u].mission != 0x47 && (int)g_units[u].mission != 0x41
                    && regstate == 0)
                    take = 1;
                if (g_units[u].type == UNIT_PIONEERS) {
                    if (routetotal == 0 && regstate != 0)
                        continue;
                    take = 1;
                }
                if (take != 0) {
                    g_units[u].orders = ORDERS_SENTRY;
                    cap -= g_unit_types[g_units[u].type].size;
                }
            }
            u = saveu;
            g_colony_tally[nation] = 0;
        }
        while (cap != 0 && aggr != 0) {
            bestcol = -1;
            best = -1;
            ringt = get_warehouse();
            for (good = 0; good < 0x10; good++) {
                amount = g_cur_colony_rec->stock[good];
                if (amount >= ringt && good != 0)
                    amount = amount * 2;
                else if (good == 8) {
                    amount = 0x19 - (ringt - amount);
                    amount = *max_ptr_1028_e5ff((t2 = 0, &t2), (t1 = amount - 2, &t1));
                }
                if (good == 5)
                    continue;
                if (good == 0xe || good == 0xf) {
                    if (isship == 0)
                        continue;
                    if (!(g_cur_colony_rec->changed & (1 << good)))
                        continue;
                    amount -= 0x64;
                }
                if (isship != 0 && good == 0xd)
                    continue;
                if (isship != 0 && good == 0)
                    continue;
                if (amount == 0)
                    continue;
                if (isship != 0)
                    d = g_goods_value[nation][good] * amount;
                else {
                    d = g_goods_value[nation][good];
                    while (d >= 2 && rand_range(0, 3) == 0)
                        d--;
                    saveu = (good == 0xd) ? 8 : 4;
                    if (d < saveu) {
                        defenders = d;
                        d = saveu - d;
                        d = amount * d;
                        d -= defenders * 5 - 5;
                    } else
                            d = -1;
                        if (amount < 0x32)
                            d = -1;
                    }
                    if (d > best) {
                        best = d;
                        bestcol = good;
                    }
                }
                if (bestcol >= 0) {
                    amount = min_int_1028_e376(0x64, g_cur_colony_rec->stock[bestcol]);
                    if (bestcol == 0 && amount > 0xa
                        && g_cur_colony_rec->stock[bestcol] < 0x6e)
                        amount -= 0xa;
                    if (bestcol == 8)
                        amount = 0x19 - (ringt - g_cur_colony_rec->stock[GOODS_HORSES]);
                    if (bestcol == 0xe || bestcol == 0xf)
                        amount = min_int_1028_e376(amount,
                        g_cur_colony_rec->stock[bestcol] - 0x64);
                    amount = max_int_1028_e350(0, amount);
                    amount = min_int_1028_e376(0x64, g_cur_colony_rec->stock[bestcol]);
                    g_cur_colony_rec->stock[bestcol] -= amount;
                    stow_cargo(u, bestcol, amount);
                    cap--;
                    if (isship != 0)
                        g_units[u].home = mycol;
                    if (isship == 0)
                        g_units[u].hold.w.trade_loaded = 1;
                } else
                        cap = 0;
                }
            }
            if (isship != 0) {
                good = g_units[u].cargo_count;
                memset_rtl(cargo, 0, 0x10);
                maxtype = -1;
                for (ci = 0; ci < good; ci++) {
                    ctype = get_cargo(u, ci);
                    if (ctype < GOODS_TRADE_GOODS && ctype != GOODS_HORSES)
                        continue;
                    if (maxtype < ctype)
                        maxtype = ctype;
                    cargo[ctype] += get_cargo_amount(u, ci);
                }
                stack_ship(u);
                nland = stack_check(u, 2) - 1;
                n3 = stack_check(u, 3);
                n4 = stack_check(u, 4);
                n6 = stack_check(u, 6);
                n5 = stack_check(u, 5);
                tot = nland - (n3 + n4 + n5);
                n3copy = n3;
                if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
                    n4 += stack_check(u, 0xc);
                relocate_stacked_units(u, x, y);
                tm7 = at_plan(nation, x, y, 7);
                tm1 = at_plan(nation, x, y, 1);
                hf = passenger(u);
                if (hf >= 0) {
                    defenders = build_another_colony(nation)
                        + unit_build_factor(nation, hf, -1);
                    if (defenders > 0) {
                        t74 = tot;
                        n3 += tot;
                        tot = 0;
                    } else if (tm1 == 0) {
                        tot += n3;
                        n3 = 0;
                    }
                }
                if (nland != 0) {
                    flags = 0;
                    if (n3 != 0 || n4 != 0 || n5 != 0) {
                        for (dd2 = 0; dd2 < 8; dd2++) {
                            tx = g_dir_dx[dd2] + x;
                            ty = g_dir_dy[dd2] + y;
                            if (on_map(tx, ty) == 0)
                                continue;
                            if (terrain_is_ocean(tx, ty) != 0)
                                continue;
                            occupant = is_anything(tx, ty);
                            if (occupant >= 0 && occupant != nation)
                                continue;
                            vregion = land_region_at(tx, ty);
                            flags = 0;
                            if (g_region_state[nation][vregion] == 0)
                                continue;
                            if (ty < 2 || g_map_size.h - 3 < ty)
                                continue;
                            if (g_units[u].orders == ORDERS_MISSION
                                && land_region_at(g_units[u].dest_x, g_units[u].dest_y) == vregion)
                                flags = -1;
                            if (n5 != 0 && g_region_state[nation][vregion] != 0) {
                                if ((g_region_colony_count[nation][vregion] < 1 && g_region_size[vregion] > 0xa)
                                    || (g_region_size[vregion] >> 3) > g_region_unit_count[nation][vregion])
                                    flags |= 0x20;
                            }
                            if (n3 != 0) {
                                if (region_build_factor(nation, vregion) > 0)
                                    flags |= 0x40;
                                if (g_region_unit_count[nation][vregion] == 0)
                                    flags |= 0x40;
                                okflag = 1;
                                if (mycol >= 0 && mycolregion == vregion) {
                                    if (8 - g_region_colony_count[nation][vregion] > mycoldist)
                                        flags &= ~0x40;
                                    if (mycoldist >= 0xc)
                                        okflag = 0;
                                }
                                if (okflag != 0) {
                                    defenders = g_game.turn_counter >> 4;
                                    if (g_unclaimed_regions != 0
                                        && (g_region_colony_count[nation][vregion] << 2)
                                        + g_region_unit_count[nation][vregion] > defenders
                                        && g_colony_tally[nation] < 0x14)
                                        flags &= ~0x40;
                                }
                                if (t74 != 0 && g_region_unit_tally[vregion] > 1)
                                    flags &= ~0x40;
                                if (tm7 != 0)
                                    flags |= 0x40;
                                if (tm1 != 0)
                                    flags |= 0x40;
                                if (g_sea_claim & (1 << vregion))
                                    flags |= 0x40;
                            }
                            if (n4 != 0 && (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
                                && g_region_colony_count[g_game.player_nation][vregion] != 0)
                                flags |= 0x10;
                            if (n4 != 0 && !(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
                                if (g_region_state[nation][vregion] == 4)
                                    flags |= 0x10;
                                if (n6 == 0) {
                                    defenders = g_game.turn_counter >> 4;
                                    if (g_unclaimed_regions != 0
                                        && (g_region_colony_count[nation][vregion] << 2)
                                        + g_region_unit_count[nation][vregion] > defenders
                                        && g_colony_tally[nation] < 0x14)
                                        flags &= ~0x10;
                                }
                                if (g_region_colony_count[nation][vregion] == 0 && (g_region_flags[vregion] & REGION_RIVAL_COLONY)
                                    && anycoldist < 7)
                                    flags |= 0x10;
                                if (g_region_flags[vregion] & REGION_GARRISONED)
                                    flags |= 0x10;
                            }
                            if (n4 != 0) {
                                if (at_plan(nation, x, y, 7) != 0)
                                    flags |= 0x10;
                                if (at_plan(nation, x, y, 1) != 0)
                                    flags |= 0x10;
                                if (g_land_claim & (1 << vregion))
                                    flags |= 0x10;
                            }
                        }
                    }
                    if (g_units_expelled != 0)
                        flags = 0;
                    if (flags != 0) {
                        if (tm1 != 0 || tm7 != 0)
                            ismission = 0;
                        saveu = u;
                        moved = 0;
                        do {
                            anymoved = 0;
                            dir = -1;
                            for (u = top_unit(saveu);
                                dir != 8 && anymoved == 0 && u >= 0;
                            u = next_unit(u)) {
                                if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
                                    dir = find_beach(nation, x, y, flags & 0x40,
                                        g_units[u].type == UNIT_ARTILLERY);
                                    if ((unsigned char)(g_unit_types[g_units[u].type].bits & flags)
                                        && dir != 8) {
                                        moved = 1;
                                        g_units[u].moves_used = 0;
                                        move_a_unit(u, dir);
                                        end_unit_turn(u);
                                        if ((char)g_units[u].x != (char)x
                                            || (char)g_units[u].y != (char)y)
                                            anymoved = 1;
                                    }
                                }
                            }
                        } while (anymoved != 0);
                        u = saveu;
                        if (moved != 0)
                            end_unit_turn(u);
                    }
                    if (ismission == 0) {
                        if (tot != 0
                            || ((nland - n3 != 0 || g_colony_tally[nation] >= 0x19) && flags == 0)) {
                            bestx = g_units[u].x;
                            besty = g_units[u].y;
                            best = -9999;
                            for (ci = 0; g_game.colony_count > ci; ci++) {
                                set_colony(ci);
                                if (g_cur_colony_rec->nation != (unsigned char)nation)
                                    continue;
                                if (g_units[u].x == g_cur_colony_rec->x
                                    && g_units[u].y == g_cur_colony_rec->y)
                                    continue;
                                vregion = land_region_at(g_cur_colony_rec->x,
                                    g_cur_colony_rec->y);
                                if (n3copy != 0 && g_region_state[nation][vregion] == 0)
                                    continue;
                                ringt = colony_max_size();
                                if (ringt > 0xc)
                                    ringt = 0x10;
                                mul = 1;
                                if (n4 != 0) {
                                    d = 0;
                                    if (g_region_state[nation][vregion] == 0)
                                        continue;
                                    defenders = g_region_flags[land_region_at(g_cur_colony_rec->x,
                                        g_cur_colony_rec->y)] & 7;
                                    defenders = defenders << 3;
                                    d += defenders;
                                    if (g_garrisoned_regions != 0 && n4 > 1)
                                        d -= (g_garrisoned_regions * n4) << 3;
                                    if (g_region_colony_count[g_game.player_nation][vregion] != 0)
                                        d += 0x32;
                                    if (g_cur_colony_rec->ai_flags & COLONY_AI_HAS_SOLDIERS)
                                        d += 0x3c;
                                    else if (g_unclaimed_regions > 1 && g_colony_tally[nation] < 0x14)
                                        d -= 0x2d;
                                    else if (g_cur_colony_rec->ai_flags & COLONY_AI_UNDER_LIMIT)
                                        d += 0x2d;
                                    else
                                        d -= 0xf;
                                } else {
                                    d = 0x11 - min_int_1028_e376(0x10, (int)g_cur_colony_rec->pop);
                                    d = d * d;
                                    d = d << 2;
                                    d += rand_range(0, 8) + 8;
                                    d += (ringt - (int)g_cur_colony_rec->pop) * 2;
                                    if (g_region_colony_count[g_game.player_nation][vregion] != 0)
                                        d += 0x14;
                                    d += (g_cur_colony_rec->ai_flags & COLONY_AI_HAS_ROOM) ? 0x19 : -0x19;
                                }
                                d += (int)g_cur_colony_rec->turns_since_ship;
                                if (g_cur_colony_rec->ai_flags & COLONY_AI_BLOCKADED) {
                                    if (g_units[u].type != UNIT_FRIGATE)
                                        d -= 0x32;
                                } else if (g_cur_colony_rec->ai_flags & COLONY_AI_BESIEGED) {
                                    if (g_units[u].type < UNIT_FRIGATE)
                                        d -= 0x14 - g_unit_types[g_units[u].type].defence * 2;
                                }
                                if (!(g_colonies[g_cur_colony_idx].status & 0x40))
                                    find_close_colony(g_cur_colony_rec->x,
                                    g_cur_colony_rec->y, nation, -2);
                                if (g_cur_colony_idx >= 0) {
                                    tx = g_cur_colony_rec->x;
                                    ty = g_cur_colony_rec->y;
                                    d = d - ((map_dist(x, y, tx, ty) * mul >> 1) + 1);
                                    if (d >= best) {
                                        best = d;
                                        bestx = tx;
                                        besty = ty;
                                    }
                                }
                            }
                            if ((n4 != 0 ? 0 : -999) < best) {
                                order_to(u, 0x34, bestx, besty);
                                goto out;
                            }
                        }
                    }
                }
no_land:
                if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && g_units[u].type == UNIT_MAN_O_WAR && ismission == 0
                    && nland == 0 && g_game.royal_forces[ROYAL_MAN_O_WAR] == 0 && g_nation_ships_in_transit[nation] == 0
                    && g_game.royal_forces[ROYAL_REGULARS] + g_game.royal_forces[ROYAL_CAVALRY] + g_game.royal_forces[ROYAL_ARTILLERY] != 0) {
                    set_course_for_europe(u);
                    goto out;
                }
                if (nland == 0 && maxtype < 0 && aggr2 != 0 && ismission == 0
                    && g_europe_land_units[nation] > g_nation_ships_in_transit[nation]) {
                    set_course_for_europe(u);
                    goto out;
                }
                if (good != 0 && ismission == 0) {
                    if (maxtype >= 0) {
                        bestcol = -1;
                        best = -1;
                        for (dd2 = 0; g_game.colony_count > dd2; dd2++) {
                            set_colony(dd2);
                            if (g_cur_colony_rec->nation != (unsigned char)nation)
                                continue;
                            if (dd2 == mycol && mycoldist == 0)
                                continue;
                            if (!(g_colonies[dd2].status & 0x40))
                                continue;
                            if ((int)g_units[u].home == dd2)
                                continue;
                            if (maxtype == 8) {
                                if (get_warehouse()
                                    < g_cur_colony_rec->stock[GOODS_HORSES] + cargo[8])
                                    continue;
                            }
                            vregion = land_region_at(g_cur_colony_rec->x,
                                g_cur_colony_rec->y);
                            d = 0;
                            cap2 = get_warehouse();
                            freeflag = 1;
                            for (good = 0; good < 0x10; good++) {
                                if (cargo[good] == 0)
                                    continue;
                                if ((g_cur_colony_rec->changed & (1 << good))
                                    && g_cur_colony_rec->stock[good] >= 0x64) {
                                    freeflag = 0;
                                    break;
                                }
                                if (g_cur_colony_rec->stock[good] + cargo[good] >= cap2)
                                    d -= (g_cur_colony_rec->stock[good] + cargo[good] - cap2)
                                    * (g_goods_value[nation][good] << 2);
                                if ((char)g_cur_colony_rec->wanted_goods == (char)good)
                                    d += ((int)g_cur_colony_rec->turns_since_ship << 2) + 0x20;
                                d += cap2 - (g_cur_colony_rec->stock[good] + 1);
                            }
                            if (freeflag == 0)
                                continue;
                            if (maxtype == 0xf) {
                                if (g_region_colony_count[g_game.player_nation][vregion] != 0)
                                    d += 0x10;
                                for (dd = 0; dd < 0x14; dd++) {
                                    nx = g_cur_colony_rec->x + g_ring_dx[dd];
                                    ny = g_cur_colony_rec->y + g_ring_dy[dd];
                                    if (on_map(nx, ny) == 0)
                                        continue;
                                    hn = is_anything(nx, ny);
                                    if (hn >= NATION_EUROPEAN_COUNT)
                                        d += piss_factor(
                                        pissed(hn - 4, nation)) << 4;
                                    else
                                        d += 0x18;
                                }
                            }
                            if (g_cur_colony_rec->ai_flags & COLONY_AI_BLOCKADED) {
                                if (g_units[u].type < UNIT_PRIVATEER)
                                    d -= 0x50 - (g_unit_types[g_units[u].type].defence << 3);
                            } else if (g_cur_colony_rec->ai_flags & COLONY_AI_BESIEGED) {
                                if (g_units[u].type < UNIT_PRIVATEER)
                                    d -= 0x14 - g_unit_types[g_units[u].type].defence * 2;
                            }
                            d = d / ((map_dist(x, y, g_cur_colony_rec->x,
                                g_cur_colony_rec->y) >> 2) + 1);
                            if (d >= best) {
                                best = d;
                                bestcol = dd2;
                            }
                        }
                        if (bestcol >= 0) {
                            set_colony(bestcol);
                            order_to(u, 0x50, g_cur_colony_rec->x,
                                g_cur_colony_rec->y);
                            goto out;
                        }
                        while (g_units[u].cargo_count != 0) {
                            good = unload_cargo(u, 0);
                            traffic_to_europe(good, g_cargo_amount);
                            g_cur_nation_rec->gold += (long)(g_goods_value[nation][good] * g_cargo_amount);
                            g_cur_nation_rec->trade_gold[good] += (long)(g_goods_value[nation][good] * g_cargo_amount);
                            g_cur_nation_rec->trade_tons[good] += (long)g_cargo_amount;
                        }
                        g_units[u].cargo_count = 0;
                    }
                    if (g_unit_types[g_units[u].type].holds - g_units[u].cargo_count == 0
                        || g_units[u].cargo_count > 1) {
                        set_course_for_europe(u);
                        goto out;
                    }
                }
            }
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST
                && (aggr != 0 || aggr2 != 0) && nland == 0 && ismission == 0) {
                bestcol = -1;
                best = -1;
                for (dd2 = 0; dd2 < 0x10; dd2++) {
                    if (g_cargo_pickups[dd2].colony < 0)
                        continue;
                    if ((int)g_cargo_pickups[dd2].loads <= 0)
                        continue;
                    set_colony(g_cargo_pickups[dd2].colony);
                    d = g_cargo_pickups[dd2].value;
                    if ((int)g_units[u].home == g_cur_colony_idx)
                        continue;
                    if ((g_cur_colony_rec->ai_flags & COLONY_AI_BLOCKADED) && g_units[u].type < UNIT_PRIVATEER)
                        continue;
                    d = d / ((map_dist(g_units[u].x, g_units[u].y,
                        g_cur_colony_rec->x,
                        g_cur_colony_rec->y) >> 2) + 1);
                    if (d >= best && (aggr != 0 || g_cargo_pickups[dd2].troops != 0)) {
                        best = d;
                        bestcol = dd2;
                    }
                }
                if (bestcol >= 0) {
                    amt = (int)g_cargo_pickups[bestcol].loads;
                    amt -= g_unit_types[g_units[u].type].holds - g_units[u].cargo_count;
                    amt = max_int_1028_e350(0, amt);
                    g_cargo_pickups[bestcol].value = g_cargo_pickups[bestcol].value * amt
                        / (int)g_cargo_pickups[bestcol].loads;
                    g_cargo_pickups[bestcol].loads = amt;
                    set_colony(g_cargo_pickups[bestcol].colony);
                    if (amt == 0)
                        g_cargo_pickups[bestcol].colony = -1;
                    if (v_da != 0)
                        ;
                    order_to(u, 0x35, g_cur_colony_rec->x,
                        g_cur_colony_rec->y);
                    goto out;
                }
            }
            if (ismission != 0)
                goto out;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && nland == 0
                && maxtype < 0 && aggr2 != 0 && ismission == 0
                && ((g_units[u].flags2 & 0x20)
                || ((g_game.turn_counter + u) & 0x1f) == 0)) {
                set_course_for_europe(u);
                goto out;
            }
            if (g_units[u].type == UNIT_WAGON_TRAIN) {
                if (g_units[u].hold.w.trade_loaded != 0) {
                    bestcol = -1;
                    best = 9999;
                    for (dd2 = 0; g_game.settlement_count > dd2; dd2++) {
                        set_village(dd2);
                        if (land_region_at(g_cur_settlement_rec->x,
                            g_cur_settlement_rec->y) != region)
                            continue;
                        dist = map_dist(x, y, g_cur_settlement_rec->x,
                            g_cur_settlement_rec->y);
                        capital = g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL;
                        if (capital != 0)
                            dist = *max_ptr_1028_e5ff((t4 = 1, &t4), (t3 = dist >> 1, &t3));
                        if (dist < best) {
                            bestcol = dd2;
                            best = dist;
                        }
                    }
                    if (bestcol >= 0) {
                        set_village(bestcol);
                        order_to(u, 0x34, g_cur_settlement_rec->x,
                            g_cur_settlement_rec->y);
                        goto out;
                    }
                    delete_unit(u);
                    goto retlbl;
                } else {
                    if (mycoldist == 0) {
                        if ((int)g_units[u].home < 0)
                            g_units[u].home = mycol;
                        if (g_units[u].home == (signed char)mycol) {
                            g_units[u].mission = 0x55;
                            dir = 8;
                            goto move;
                        }
                    }
                    if ((int)g_units[u].home >= 0) {
                        set_colony((int)g_units[u].home);
                        order_to(u, 0x35, g_cur_colony_rec->x,
                            g_cur_colony_rec->y);
                        goto out;
                    }
                    if (region == mycolregion) {
                        g_units[u].home = mycol;
                        set_colony(mycol);
                        order_to(u, 0x35, g_cur_colony_rec->x,
                            g_cur_colony_rec->y);
                        goto out;
                    }
                    delete_unit(u);
                    goto retlbl;
                }
            }
            if (g_units[u].type == UNIT_TREASURE_TRAIN) {
                if (mycoldist == 0) {
                    amountl = (unsigned char)g_units[u].spec;
                    amountl *= 0x64;
                    g_cur_nation_rec->gold += (unsigned)amountl;
                    if (!(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
                        popup_say(0, get_adjective(nation));
                        popup_say(1, g_nation_name_word[nation]);
                        set_text_arg_u_1028_e3ca(0, amountl);
                        popm("LOOTFOREIGN", 2);
                    }
                    delete_unit(u);
                    goto retlbl;
                } else {
                    if (region == mycolregion) {
                        set_colony(mycol);
                        order_to(u, 0x35, g_cur_colony_rec->x,
                            g_cur_colony_rec->y);
                        goto out;
                    }
                    saveu = u;
                    u = find_my_close_unit(nation, u, x, y);
                    if (u >= 0
                        && land_region_at(g_units[u].x, g_units[u].y) == region) {
                        nx = g_units[u].x;
                        ny = g_units[u].y;
                        u = saveu;
                        order_to(u, 0x21, nx, ny);
                        goto out;
                    }
                    u = saveu;
                    if (menaced(x, y, nation, 0) == g_game.player_nation) {
                        delete_unit(u);
                        goto retlbl;
                    }
                }
            }
            if (g_units[u].type == UNIT_MISSIONARIES) {
                bestcol = -1;
                best = -999;
                for (dd2 = 0; g_game.settlement_count > dd2; dd2++) {
                    set_village(dd2);
                    if (land_region_at(g_cur_settlement_rec->x,
                        g_cur_settlement_rec->y) != region)
                        continue;
                    if ((g_cur_settlement_rec->mission & 0xf) == nation) {
                        if (g_cur_nation_rec->gold < 2500L)
                            continue;
                        if (pissed(g_cur_tribe, g_game.player_nation) >= TRIBE_ATTITUDE_WAR)
                            continue;
                        if (!(get_treaty(g_cur_tribe, g_game.player_nation) & 0x20))
                            continue;
                        if (g_rank[nation] >= g_rank[g_game.player_nation])
                            continue;
                    }
                    capital = g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL;
                    infl = village_anger(dd2, &cell);
                    if (infl >= 0)
                        score2 += cell << 5;
                    score2 = pissed(g_cur_tribe, nation);
                    score2 = score2 << 3;
                    score2 = score2 / (map_dist(x, y,
                        g_cur_settlement_rec->x,
                        g_cur_settlement_rec->y) + 1);
                    if (capital != 0)
                        score2 += score2 >> 1;
                    if (score2 > best) {
                        best = score2;
                        bestcol = dd2;
                    }
                }
                if (bestcol >= 0) {
                    set_village(bestcol);
                    order_to(u, 0x4a, g_cur_settlement_rec->x,
                        g_cur_settlement_rec->y);
                    goto out;
                }
                g_units[u].type = UNIT_COLONISTS;
            }
            if ((g_units[u].type == UNIT_SCOUTS || g_units[u].type == UNIT_PIONEERS)
                && regstate == 0 && mycolregion == region) {
                set_colony(mycol);
                order_to(u, 0x4e, g_cur_colony_rec->x,
                    g_cur_colony_rec->y);
                goto out;
            }
            if (wantwork != 0) {
                if (g_units[u].orders == ORDERS_MISSION)
                    goto out;
                if ((int)g_units[u].hold.w.work_dir < 0)
                    g_units[u].hold.w.work_dir = (unsigned char)(rand_range(1, 0x14) - 1);
                dd2 = (unsigned char)g_units[u].hold.w.work_dir;
                if ((int)g_units[u].mission == 0x38 && g_units[u].hold.w.work_turns != 0) {
                    tx = g_units[u].dest_x;
                    ty = g_units[u].dest_y;
                } else {
                    tx = (g_ring_dx[dd2] << 2) + x;
                    ty = (g_ring_dy[dd2] << 2) + y;
                }
                if (on_map(tx, ty) != 0 && !(g_sketch[tx >> 2][ty >> 2] & 6)
                    && land_region_at(tx, ty) == region
                    && is_anything(tx, ty) < 0) {
                    g_units[u].hold.w.work_turns = *max_ptr_1028_e5ff((t6 = g_ring_dx[dd2] << 2, &t6),
                        (t5 = g_ring_dy[dd2] << 2, &t5));
                    if (g_units[u].hold.amount[0] > 8)
                        g_units[u].hold.amount[0] -= 8;
                    order_to(u, 0x38, tx, ty);
                    goto out;
                }
            }
            if ((g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                && g_unit_types[g_units[u].type].attack > 1
                && (int)g_units[u].home >= 0) {
                set_colony((int)g_units[u].home);
                if ((g_cur_colony_rec->ai_flags & COLONY_AI_OVER_LIMIT)
                    && (g_cur_colony_rec->want_defenders_threat != 0 || g_units[u].type != UNIT_DRAGOONS)
                    && land_region_at(g_cur_colony_rec->x,
                    g_cur_colony_rec->y) == region) {
                    g_cur_colony_rec->ai_flags &= ~COLONY_AI_OVER_LIMIT;
                    if (g_cur_colony_rec->want_defenders_threat != 0)
                        g_cur_colony_rec->want_defenders_threat--;
                    order_to(u, 0x57, g_cur_colony_rec->x,
                        g_cur_colony_rec->y);
                    goto out;
                }
            }
            if (g_units[u].type == UNIT_PIONEERS && wantwork == 0) {
                freeflag = 1;
                if (vill >= 0) {
                    set_village(vill);
                    lvl = tribe_radius(g_cur_tribe);
                    if (villdist <= lvl && attq < 3)
                        freeflag = 0;
                }
                if (anycol >= 0) {
                    set_colony(anycol);
                    if (g_cur_colony_rec->nation != (unsigned char)nation && anycoldist < 3)
                        freeflag = 0;
                }
                if (freeflag != 0) {
                    g_units[u].orders = ORDERS_BUILD_ROAD;
                    g_units[u].mission = 0x52;
                    goto out;
                }
            }
            if (g_units[u].orders != ORDERS_NONE && g_units[u].orders != 0xa
                && g_units[u].orders != ORDERS_FORTIFY && g_units[u].orders != ORDERS_FORTIFIED) {
                if (g_units[u].orders == ORDERS_MISSION && (char)g_units[u].dest_x == (char)x
                    && (char)g_units[u].dest_y == (char)y)
                    goto after_all;
                if (zoc_4(x, y, nation) == 0)
                    goto out;
            }
after_all:
            tm1 = 0;
            if (menaced(x, y, nation, 1) < 0
                || ((g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                && g_unit_types[g_units[u].type].attack == 0))
                tm1 = 1;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && (g_game.game_flags & GAMEFLAG_INDEPENDENCE))
                tm1 = 0;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && tm1 != 0
                && n3 + n4 != 0 && flags == 0 && !(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
                if (!(g_units[u].flags2 & 0x10)) {
                    if (rand_range(0, 0x10) == 0) {
                        tx = rand_range(2, g_map_size.w - 3);
                        ty = rand_range(2, g_map_size.h - 3);
                        if (terrain_is_ocean(tx, ty) != 0 && continent_at(tx, ty) == 1
                            && map_dist(x, y, tx, ty) >= 8) {
                            g_units[u].flags2 |= 0x10;
                            order_to(u, 0x44, tx, ty);
                            goto out;
                        }
                    }
                } else {
                    if (rand_range(0, 0x30) == 0)
                        g_units[u].flags2 &= ~0x10;
                }
            }
            best = -999;
            dir = 8;
            for (dd2 = 0; dd2 < 8; dd2++) {
                tx = g_dir_dx[dd2] + x;
                ty = g_dir_dy[dd2] + y;
                if (on_map(tx, ty) == 0)
                    continue;
                terrain2 = terrain_at(tx, ty);
                if (terrain2 == TERRAIN_OCEAN || terrain2 == TERRAIN_SEA_LANE) {
                    if (isship == 0)
                        continue;
                    if (continent_at(tx, ty) != 1)
                        continue;
                }
                hn = owner_of(tx, ty);
                occupant = top_unit_in_square(tx, ty);
                if (iswater != 0 && isship == 0 && occupant >= 0 && hn != nation)
                    continue;
                if (isship != 0 && terrain2 != TERRAIN_OCEAN && terrain2 != TERRAIN_SEA_LANE) {
                    if (is_colony(tx, ty) < 0)
                        continue;
                    if (g_units[u].type != UNIT_FRIGATE)
                        continue;
                    continue;
                }
                if (g_units[u].type == UNIT_SCOUTS) {
                    d = rand_range(1, 8);
                    if (river != 0 && (map_get(tx, ty) & MAP0_RIVER) && !(dd2 & 1))
                        d += 2;
                    else if (improved != 0 && (feature_get(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD)))
                        d++;
                    else
                        d -= g_terrain_rows[terrain2].movement * 3;
                } else if (wantwork != 0)
                        d = (rand_range(1, 4) + (site_get(tx, ty) & 0xf)) >> 1;
                    else if (!(g_units[u].flags >> 4) && iswater == 0) {
                        if ((g_unit_types[g_units[u].type].bits & 0x20)
                            || (g_unit_types[g_units[u].type].bits & 0x10)) {
                            d = rand_range(1, 3);
                            if (river != 0 && (map_get(tx, ty) & MAP0_RIVER) && !(dd2 & 1))
                                d++;
                            else if (improved != 0 && (feature_get(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD)))
                                d++;
                            else
                                d -= g_terrain_rows[terrain2].movement;
                        } else {
                            d = rand_range(1, 3);
                            if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
                                d -= g_terrain_rows[terrain2].defence;
                            else
                                d += g_terrain_rows[terrain2].defence;
                        }
                    } else {
                        d = rand_range(1, 5);
                        if (occupant >= 0 && hn == nation)
                            ;
                        else
                            d += g_terrain_rows[terrain2].defence << 2;
                    }
                    if (terrain2 == TERRAIN_SEA_LANE)
                        d -= 0x10;
                    if ((g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                        && g_unit_types[g_units[u].type].attack > 1
                        && is_colony(tx, ty) >= 0) {
                        if (nation == hn) {
                            set_colony(mycol);
                            if (g_cur_colony_rec->ai_flags & COLONY_AI_HAS_SOLDIERS)
                                d += 0xa;
                            else if (g_cur_colony_rec->ai_flags & COLONY_AI_OVER_LIMIT)
                                d += 6;
                            else if (g_cur_colony_rec->ai_flags & COLONY_AI_HAS_ROOM)
                                d += 3;
                        } else
                                d += 0x10;
                        }
                        attacked = 0;
                        if (occupant >= 0 || is_anything(tx, ty) >= 0) {
                            if (hn != nation) {
                                if (hn >= NATION_EUROPEAN_COUNT) {
                                    if (pissed(hn - 4, nation) < TRIBE_ATTITUDE_WAR
                                        && !(get_treaty(nation, hn) & 2))
                                        continue;
                                    if (get_treaty(nation, hn) & 2)
                                        d = d * 2;
                                    if (g_region_colony_count[nation][region] == 0)
                                        continue;
                                } else {
                                    if ((get_treaty(nation, hn) & 0x40)
                                        && g_units[u].type != UNIT_PRIVATEER
                                        && g_units[occupant].type != UNIT_PRIVATEER)
                                        continue;
                                    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
                                        if (hn >= NATION_EUROPEAN_COUNT || g_nation_text[hn].control != CONTROL_HUMAN) {
                                            if (hn < NATION_EUROPEAN_COUNT)
                                                continue;
                                        }
                                    }
                                }
                                if (g_unit_types[g_units[u].type].attack == 0)
                                    continue;
                                attacked = 1;
                                v7a = 1;
                                res = combat_fight(u, tx, ty, 0, 0);
                                res = (stack_check(occupant, 0) + 1)
                                    / max_int_1028_e350(1, stack_check(occupant, 2))
                                    * res;
                                res = res / max_int_1028_e350(1, g_unit_types[g_units[u].type].hammers);
                                bonus = 0;
                                if (is_colony(tx, ty) >= 0) {
                                    res *= 3;
                                    bonus = 1;
                                }
                                if (is_village(tx, ty) >= 0) {
                                    res *= 2;
                                    bonus = 1;
                                }
                                if (g_units[u].type == UNIT_ARTILLERY && bonus == 0)
                                    res = 0;
                                if (g_game.mother_country_idx == nation && bonus == 0 && mycoldist == 0)
                                    res >>= 1;
                                if ((g_unit_types[g_units[u].type].bits & 0x10) && regstate == 4)
                                    res = res * 3;
                                if ((g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS)
                                    && is_colony(tx, ty) >= 0) {
                                    art = stack_check(occupant, 0xb);
                                    if (art != 0) {
                                        mine = 0;
                                        saveu = u;
                                        for (dd = 0; dd < 8; dd++) {
                                            nx2 = g_dir_dx[dd] + tx;
                                            ny2 = g_dir_dy[dd] + ty;
                                            u = top_unit_in_square(nx2, ny2);
                                            if (u >= 0
                                                && (unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == (unsigned char)nation)
                                                mine += stack_check(u, 0xb);
                                        }
                                        u = saveu;
                                        if (mine <= art)
                                            continue;
                                    }
                                }
                                if (res >= 0x3e8 || res < 0)
                                    res = 0x3e8;
                                if (res >= 0xc
                                    || (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST))
                                    d += max_int_1028_e350(1, res) << 2;
                                else
                                    d -= 0x3e7;
                            } else
                                    ;
                            } else
                                    ;
                                if ((int)g_units[u].last_dir >= 0 && (int)g_units[u].last_dir < 8) {
                                    delta = ABS((int)g_units[u].last_dir - dd2);
                                    if (delta > 4)
                                        delta = 8 - delta;
                                    d -= delta * delta * 2;
                                }
                                for (dd = 0; dd < 8; dd++) {
                                    nx2 = g_dir_dx[dd] + tx;
                                    ny2 = g_dir_dy[dd] + ty;
                                    hn = is_unit(nx2, ny2);
                                    if (hn >= 0 && hn != nation
                                        && (get_treaty(nation, hn) & 0x60) == 0x20
                                        && g_unit_types[g_units[u].type].attack == 0) {
                                        for (uu = top_unit_in_square(nx2, ny2); uu >= 0; uu = next_unit(uu)) {
                                            if (g_unit_types[g_units[uu].type].attack != 0)
                                                d -= 0xa;
                                        }
                                    }
                                    if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                                        col = find_colony(nx2, ny2);
                                        if (col >= 0) {
                                            hn = g_colonies[col].nation;
                                            pen = 0;
                                            if ((get_treaty(nation, hn) & 0x60) == 0x20) {
                                                if (colony_has_building(col, 1))
                                                    pen = 0x14;
                                                if (colony_has_building(col, 2))
                                                    pen = 0x28;
                                                for (defenders = top_unit_in_square(nx2, ny2); defenders >= 0;
                                                    defenders = next_unit(defenders)) {
                                                    if (g_units[defenders].type == UNIT_ARTILLERY)
                                                        pen += 0x1e;
                                                }
                                                pen = g_units[u].cargo_count * pen;
                                                d -= pen;
                                            }
                                        }
                                    }
                                }
                                if (tm1 != 0) {
                                    tx = (g_dir_dx[dd2] << 2) + x;
                                    ty = (g_dir_dy[dd2] << 2) + y;
                                    if (g_sketch[tx >> 2][ty >> 2] == 0 && terrain_is_ocean(tx, ty) == 0
                                        && on_map(tx, ty) != 0)
                                        d += 8;
                                    if (isship != 0 && dd2 >= 5 && dd2 <= 7 && (g_map_size.w >> 1) < x)
                                        d += 4;
                                    for (dd = 0; dd < 8; dd++) {
                                        nx2 = g_dir_dx[dd] + tx;
                                        ny2 = g_dir_dy[dd] + ty;
                                        if (on_map(nx2, ny2) == 0)
                                            continue;
                                        if (nation < NATION_EUROPEAN_COUNT && !((0x10 << nation) & site_get(nx2, ny2))
                                            && (terrain_is_ocean(nx2, ny2) == 0 || isship != 0))
                                            d += 2;
                                        if (is_unit(nx2, ny2) >= 0)
                                            d -= 2;
                                        if (wantwork != 0)
                                            d += g_terrain_rows[terrain_at(nx2, ny2)].value;
                                    }
                                }
                                if (d > best) {
                                    best = d;
                                    dir = dd2;
                                    attackdir = attacked;
                                }
                            }
                            if (attackdir != 0) {
                                if (move_rate(u) - g_units[u].moves_used < 3)
                                    dir = 8;
                                goto after_move_pick;
                            }
                            if (retflag != 0) {
                                set_colony(mycol);
                                g_cur_colony_rec->want_defenders--;
                                g_units[u].mission = 0x47;
                                dir = 8;
                                goto move;
                            }
                            if ((g_unit_types[g_units[u].type].bits & 1) && mycoldist != 0) {
                                tblmax = at_plan(nation, x, y, 0);
                                if (tblmax != 0 && tblmax <= 4 && stack_check(u, 2) < 2) {
                                    g_units[u].mission = 0x42;
                                    dir = 8;
                                    goto move;
                                }
                            }
                            if ((g_unit_types[g_units[u].type].bits & 4)
                                && at_plan(nation, x, y, 2) != 0
                                && stack_check(u, 2) < 2) {
                                g_units[u].mission = 0x65;
                                dir = 8;
                                goto move;
                            }
                            if (g_unit_types[g_units[u].type].attack > 1
                                && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                                && v7a != 0
                                && top_unit(u) == bottom_unit(u)) {
                                for (dd2 = 0; dd2 < 8; dd2++) {
                                    tx = g_dir_dx[dd2] + x;
                                    ty = g_dir_dy[dd2] + y;
                                    hn = is_colony(tx, ty);
                                    if (hn < 0 || hn == nation)
                                        continue;
                                    g_units[u].mission = 0x46;
                                    dir = 8;
                                    goto move;
                                }
                            }
after_move_pick:
                            g_units[u].mission = 0x39;
                            goto move;
move:
                            g_units[u].last_dir = dir;
                            if (dir == 8) {
                                if (g_units[u].orders != ORDERS_FORTIFY && g_units[u].orders != ORDERS_FORTIFIED)
                                    g_units[u].orders = ORDERS_FORTIFY;
                                if (g_units[u].flags2 & 2)
                                    g_units[u].orders = ORDERS_FORTIFIED;
                            } else {
                                x += g_dir_dx[dir];
                                y += g_dir_dy[dir];
                                if (on_map(x, y) != 0) {
                                    g_units[u].orders = 0xc;
                                    g_units[u].dest_x = x;
                                    g_units[u].dest_y = y;
                                }
                            }
extern "C" out: if (g_units[u].orders == 0xa || g_units[u].orders == ORDERS_NONE);
                            if (g_units[u].orders == ORDERS_FORTIFY) {
                                x = g_units[u].x;
                                y = g_units[u].y;
                                for (dd2 = 0; dd2 < 8; dd2++) {
                                    tx = g_dir_dx[dd2] + x;
                                    ty = g_dir_dy[dd2] + y;
                                    hn = is_colony(tx, ty);
                                    if (hn < 0 || hn == nation)
                                        continue;
                                    if (!(get_treaty(nation, hn) & 0x40))
                                        continue;
                                    g_units[u].orders = ORDERS_NONE;
                                    break;
                                }
                            }
                            if (g_units[u].orders == ORDERS_MISSION && g_units[u].dest_x == g_units[u].x
                                && g_units[u].dest_y == g_units[u].y) {
                                if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST
                                    && (int)g_units[u].mission == 0x31)
                                    g_units[u].mission = 0x42;
                                end_unit_turn(u);
                            }
                            ok = 0;
retlbl:
                            return ok;
                        }
