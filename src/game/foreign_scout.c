/* foreign_scout: candidate for 1020:8e1e (747 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 747, not the census 738. The trailing 9 bytes are the dead second epilogue.
 *
 * A scout reaching another power's colony (the current colony). A European player is asked
 * "SCOUTCOLONY" with the colony's name; anyone else takes answer 3, which does nothing. The result
 * is 1 whenever an answer other than 3 was given.
 *
 *   answer 1  refused with "NOMAYORSDURINGREV" once bit 0 of SEG20:0x7784 is set,
 *             otherwise 1030:5240 runs for the two nations, the scout and its direction from the
 *             colony
 *   answer 0 or 2  the scout is caught when `rand_range(1, 36)` comes up at or below 12 + two
 *             per fortification level (stockade, fort, fortress) -- halved for specialty 0x16 -- plus the difficulty byte
 *             less 2 for a European player. Caught, "LOSTOURSCOUTS" or "LOSTTHEIRSCOUTS" is shown,
 *             the colony's word +0xaa grows by 100 and the scout is removed; otherwise the colony
 *             window opens on the colony in display mode 1 with DGROUP:0xa08 held for the duration.
 *
 * The random generator is reseeded from DGROUP:0x4b86 before the roll.
 *
 * RENAMED 2026-09-26 from scout_enters_colony: the Mac build's foreign_scout(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller foreign_city; callee Show; callee building_level; callee delete_unit; callee negotiate_europe), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "gameflag.h"
#include "building.h"
#include "nations.h"
#include "jobs.h"
#include "goods.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

typedef struct { char pad[1]; } ColonyWin;

extern int popm(char *msg, int n);
extern int which_move(int dx, int dy);
extern void negotiate_europe(int nation, int their_nation, int u, int dir, int flag);
extern int building_level(int bit);
extern void random_activate(int seed);

extern void popup_say(int slot, int word);
extern void delete_unit(int u);
extern void DrawMap(int flag);
extern void ColonyWin_Show(void *w, int colony);

/* same object: 4-byte `push cs; call ` (rule 47) */

int foreign_scout(register int u)
{
    int r;
    int choice;
    int un;
    int cn;
    int caught;
    int dx;
    int dy;
    int dir;
    register int chance;

    r = 0;
    un = (unsigned char)(g_units[u].flags & 0xf);
    cn = g_cur_colony_rec->nation;
    if (un < 4 && g_nation_text[un].control == 0) {
        set_name_slot_1020_a94a(0, g_cur_colony_rec->name);
        choice = popm("SCOUTCOLONY", 3);
    } else {
        choice = 3;
    }
    if (choice != 3)
        r = 1;
    if (choice >= 3)
        goto out;
    if (choice == 1) {
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
            popm("NOMAYORSDURINGREV", 1);
            goto out;
        }
        dx = g_cur_colony_rec->x - g_units[u].x;
        dy = g_cur_colony_rec->y - g_units[u].y;
        dir = which_move(dx, dy);
        negotiate_europe(un, cn, u, dir, 1);
    } else {
        chance = 0xc;
        chance += building_level(BUILDING_STOCKADE) * 2;
        if ((int)g_units[u].spec == JOB_SCOUT)
            chance = chance >> 1;
        chance += (g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == 0)
                  ? g_game.difficulty - 2 : 0;
        random_activate(g_random_seed);
        caught = rand_range(1, 0x24) <= chance;
        if (caught != 0) {
            set_name_slot_1020_a94a(1, g_cur_colony_rec->name);
            set_text_arg_i_1020_a926(0, 100);
            if (un < 4 && g_nation_text[un].control == 0) {
                popup_say(0, get_adjective(cn));
                popm("LOSTOURSCOUTS", 3);
            } else if (cn < 4 && g_nation_text[cn].control == 0) {
                popup_say(0, get_adjective(un));
                popm("LOSTTHEIRSCOUTS", 3);
            }
            g_cur_colony_rec->stock[GOODS_HORSES] += 100;
            delete_unit(u);
            DrawMap(1);
        } else {
            g_scout_visit = 1;
            g_colony_mode = 1;
            ColonyWin_Show(&g_colony_window, g_cur_colony_idx);
            g_scout_visit = 0;
        }
    }
out:
    return r;
}
