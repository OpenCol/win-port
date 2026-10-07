/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 120, 131 of the imported file. */
/* perform_orders_route: candidate for 1028:b2a1 (1159 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * ONE STEP OF A UNIT'S TRADE ROUTE. `GameBrains_ProcessActiveUnit` calls it for a unit under
 * ORDERS_TRADE_ROUTE (`at_stop` 0), and `orders_route` when the route starts (1). The name
 * describes only the last check. It is kept because many files and docs already use it.
 *
 * A unit no longer under ORDERS_TRADE_ROUTE has its orders cleared. Otherwise the route and the
 * current stop are made current; a stop's site is a colony index, or 999 for Europe.
 *
 * NOT THERE YET (`in_port` false). The unit is sent on: in Europe it sets sail back, and
 * otherwise it heads for the stop. If it is at a stop already, or on a colony or village square (`is_city`), or
 * off the map in Europe, its destination is first set to the colony square, or it heads for a sea lane.
 *
 * AT THE STOP:
 *   - UNLOAD every goods the stop lists for unloading (nibbles 0..), except goods the nation has
 *     flagged (`boycott`): sold on the Europe dock, or put into the colony.
 *   - LOAD. In Europe it buys each goods the stop lists for loading (nibbles 6..). In a colony, while the
 *     unit has hold space and the last load succeeded, it takes the listed goods worth most, scored as the
 *     nation's goods value times the colony's stock.
 *   - The unit's stop advances, wrapping at the route's stop count (+0x21).
 * ROUTELOOP: when every stop of the route names the same site, the route's name is shown with
 * ROUTELOOP and the unit is re-rated.
 *
 * It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from trade_route_single_port_check: the Mac build's perform_orders_route(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee boycott; callee on_map; callee popm; callee set_colony; callee set_europe), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

typedef struct { char pad[1]; } EuropeWin;

extern int get_unit_route(int u);
extern void set_dest(int r);
extern int get_unit_dest(int u);

extern int is_city(int x, int y);
extern void set_course_for_europe(int unit);
extern void set_colony(int colony);
extern void set_course_for_new_world(int u);
extern void set_europe(int n);
extern void end_unit_turn(int u);
extern int get_num_orders(int side);
extern int get_order(int slot);
extern int boycott(int bit);
extern int find_cargo(int u, int goods);
extern void EuropeWin_AttemptCargoToEurope(EuropeWin *w, int u, int goods, int ask);
extern int attempt_cargo_to_colony(int u, int type, int n);
extern void EuropeWin_AttemptCargoFromEurope(EuropeWin *w, int u, int goods, int allowed, int ask);

extern void sort_16x16(int n, int *payload, int *key);
extern int attempt_cargo_to_ship(int u, int goods, int a, int b);
extern void set_unit_dest(int u, int v);
extern int popm(char *msg, int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int in_port(int u, int site);
extern void perform_orders_move_to(int u);

void perform_orders_route(int u, int at_stop)
{
    int site;
    int stop;
    int count;
    int i;
    int differs;
    int arrived;
    int slot;
    int more;
    int idx[16];
    int score[16];
    register int good;

    if (g_units[u].orders != ORDERS_TRADE_ROUTE) {
        g_units[u].orders = ORDERS_NONE;
        goto out;
    }
    set_dest(get_unit_route(u));
    stop = get_unit_dest(u);
    set_route(stop);
    site = g_cur_route_stop->colony;
    if (in_port(u, site) == 0) {
        arrived = at_stop;
        if (on_map(g_units[u].x, g_units[u].y))
            arrived |= is_city(g_units[u].x, g_units[u].y) >= 0 ? 1 : 0;
        else
            arrived |= in_port(u, 999);
        if (arrived) {
            if (site == 999)
                set_course_for_europe(u);
            else {
                set_colony(site);
                g_units[u].dest_x = g_cur_colony_rec->x;
                g_units[u].dest_y = g_cur_colony_rec->y;
            }
        }
        if (in_port(u, 999))
            set_course_for_new_world(u);
        else
            perform_orders_move_to(u);
        goto out;
    }
    if (site == 999) {
        set_europe(NATION(g_units[u]));
        end_unit_turn(u);
    } else
        set_colony(site);
    count = get_num_orders(0);
    for (i = 0; i < count; i++) {
        good = get_order(i);
        if (boycott(good) == 0) {
            do {
                slot = find_cargo(u, good);
                if (slot >= 0) {
                    if (site == 999)
                        EuropeWin_AttemptCargoToEurope((EuropeWin *)(&g_europe_win), u, good, 0);
                    else
                        attempt_cargo_to_colony(u, good, 0);
                }
            } while (slot >= 0);
        }
    }
    if (site == 999) {
        count = get_num_orders(1);
        for (i = 0; i < count; i++) {
            good = get_order(i + 6);
            EuropeWin_AttemptCargoFromEurope((EuropeWin *)(&g_europe_win), u, good, 1, 0);
        }
    } else {
        more = 1;
        while (g_unit_types[g_units[u].type].holds - g_units[u].cargo_count && more) {
            memset_rtl(score, 0, 0x20);
            for (i = 0; i < 16; i++)
                idx[i] = (unsigned char)i;
            count = get_num_orders(1);
            for (i = 0; i < count; i++) {
                good = get_order(i + 6);
                score[good] = g_goods_value[NATION(g_units[u])][good] * g_cur_colony_rec->stock[good];
            }
            sort_16x16(16, idx, score);
            more = 0;
            if (score[15] != 0) {
                good = idx[15];
                more = !attempt_cargo_to_ship(u, good, 1, 0);
            }
        }
    }
    stop = (stop + 1) % g_cur_route_rec->count;
    set_unit_dest(u, stop);
    differs = 0;
    for (i = 1; g_cur_route_rec->count > i; i++)
        if (g_cur_route_rec->stops[i].colony != g_cur_route_rec->stops[0].colony)
            differs = 1;
    if (!differs) {
        set_name_slot_1028_e412(0, g_cur_route_rec->name);
        popm("ROUTELOOP", 0);
        end_unit_turn(u);
    }
out:
    ;
}
