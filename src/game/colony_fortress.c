/* colony_fortress: candidate for 1018:9117 (745 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The current colony's fortifications fire on enemy ships in the eight squares around it. A
 * colony with neither of the two fort buildings (colony flags 1 and 2) does nothing; otherwise the
 * guns' strength is (1 + artillery in the colony) * forts * 4, and the fort's name word is the one
 * for flag 2 when it is set (SEG20:0xd43a), otherwise for flag 1 (0xd42e).
 *
 * In each adjacent ocean square the first ship (types 0xd..0x12) is the target, unless it belongs
 * to the colony's own nation, or to one it holds pair flag 0x40 with and it is not type 0x10. A
 * scratch combatant of the type byte at SEG20:0x7b64 is created on the colony at that strength --
 * the combat record at SEG20:0x7c0a is given the strength at +0xb, the fort's name at +0, 8 at +4
 * -- and fought against the square through 1020:4460. When the viewing nation can see it
 * (`in_view` for the colony's square, or the ship's bit for that nation) the view scrolls to the
 * colony and "FORTFIRE" names the fort, the colony and the ship's nation. The scratch unit is freed
 * after each shot.
 *
 * RENAMED 2026-09-26 from fort_opens_fire: the Mac build's colony_fortress(void), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller new_turn_colony; callee get_treaty; callee has_building; callee next_unit; callee popm), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "building.h"
#include "units.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern int terrain_is_ocean(int x, int y);
extern int get_treaty(int nation_a, int nation_b);
extern void popup_say(int slot, int word);
extern int in_view(int x, int y, int nation);
extern int make_fake_unit(int icon, int x, int y, int owner, int strength);
extern int possibly_center(int x1, int y1, int x2, int y2, int mode);

extern int popm(char *msg, int n);
extern int combat_fight(int u, int x, int y, int seen, int f);
extern void blow_fake_unit(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int has_building(int bit);

void colony_fortress(void)
{
    int y;
    int nx;
    int ny;
    int dir;
    int arty;
    int forts;
    int power;
    int cnat;
    int snat;
    int seen;
    int fortname;
    register int u;
    register int x;

    arty = 1;
    forts = 0;
    if (has_building(BUILDING_FORT)) {
        forts++;
        fortname = g_buildings[BUILDING_FORT].name;
    }
    if (has_building(BUILDING_FORTRESS)) {
        forts++;
        fortname = g_buildings[BUILDING_FORTRESS].name;
    }
    x = g_cur_colony_rec->x;
    y = g_cur_colony_rec->y;
    cnat = g_cur_colony_rec->nation;
    for (u = top_unit_in_square(x, y); u >= 0; u = next_unit(u))
        if (g_units[u].type == UNIT_ARTILLERY)
            arty++;
    power = arty * forts * 4;
    if (power == 0)
        goto out;
    for (dir = 0; dir < 8; dir++) {
        nx = g_dir_dx[dir] + x;
        ny = g_dir_dy[dir] + y;
        if (terrain_is_ocean(nx, ny) == 0)
            continue;
        for (u = top_unit_in_square(nx, ny); u >= 0 && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST);
             u = next_unit(u))
            ;
        if (u < 0)
            continue;
        snat = (unsigned char)(g_units[u].flags & 0xf);
        if (snat == cnat)
            continue;
        if ((get_treaty(cnat, snat) & 0x40) && g_units[u].type != UNIT_PRIVATEER)
            continue;
        popup_say(3, g_unit_types[g_units[u].type].name);
        seen = in_view(x, y, g_game.view_nation)
               || (g_units[u].flags & UNIT_SEEN_BY(g_game.view_nation));
        u = make_fake_unit(g_unit_types[UNIT_ARTILLERY].icon, x, y, cnat, power);
        g_unit_types[UNIT_TEMPORARY].guns = power;
        g_unit_types[UNIT_TEMPORARY].name = (unsigned char)fortname;
        g_unit_types[UNIT_TEMPORARY].moves = 8;
        g_units[u].moves_used = 0;
        if (seen) {
            possibly_center(x, y, x, y, 0);
            popup_say(0, fortname);
            set_name_slot_1018_b3f6(1, (char *)g_cur_colony_rec + 2);
            popup_say(2, get_adjective(snat));
            popm("FORTFIRE", 0);
        }
        combat_fight(u, nx, ny, seen, 1);
        blow_fake_unit();
    }
out:
    ;
}
