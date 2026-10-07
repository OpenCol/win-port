/* give_me_liberty: candidate for 1020:9d92 (1228 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A FOUNDING FATHER JOINS THE CONGRESS and his effect is applied. `1020:a6c3` is the only caller.
 * A father of -1 is the "no father" case, which only names the nation.
 *
 * The father's bit is set for the nation, and SEG20:0x77ab remembers the FIRST nation to get him. A
 * human-played European sees the FREEDOM message and the father's Colonizopedia page.
 *
 * WHAT EACH FATHER DOES here (the rest are bits other code tests):
 *  - Jakob Fugger: clears the nation's boycotts (+0x20 of the market record);
 *  - La Salle: every colony of the nation with 3 or more colonists gets its flag 0;
 *  - John Paul Jones: a frigate (type 0x11) appears, its orders cleared and its destination set to the
 *    nation's +0x32/+0x33. The type is chosen by a test that can only be true, so the 0xf arm is dead;
 *  - Pocahontas: every tribe's anger with this nation is cancelled by an equal and opposite change, and
 *    every village's anger entry for it is zeroed;
 *  - Simon Bolivar: +20 rebel sentiment (SEG20:0x77d2), capped at 100;
 *  - Jean de Brebeuf: every mission of this nation gains bit 0x10, the expert flag;
 *  - Bartolome de las Casas: every converted native (speciality 0x1b) becomes a free colonist (0x1c),
 *    both as a unit on the map and inside every colony;
 *  - Francisco de Coronado: the map around every colony -- of any nation -- is revealed;
 *  - William Brewster: criminals and servants (0x19, 0x1a) waiting in the recruit pool become free
 *    colonists.
 * Every path ends by restoring the colony that was current and redrawing the map.
 *
 * The market's counter at +0x14 goes up and its +0x12 is cleared to -1 on every call, father or not.
 *
 * Spellings: the record pointers at DGROUP:0x4b9e, 0x4c28, 0x4c2c and 0x4cc4 are all declared FAR here;
 * the colony's population and the unit's speciality are SIGNED bytes; nation comparisons are byte
 * compares; and the loops mix `continue` (the two-jump form) with structured `&&` tests exactly where
 * the shipped jumps say.
 *
 * RENAMED 2026-09-26 from found_father_joins: the Mac build's give_me_liberty(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller add_liberty; callee force_mood; callee get_specialty; callee popup_say; callee set_colony), the runner-up gets 4, and the arity agrees (3).
 */

#include "colonization.h"

#include "control.h"
#include "building.h"
#include "nations.h"
#include "settle.h"
#include "jobs.h"
#include "units.h"
#include "fathers.h"
#include "w16types.h"
#include "classes.h"

#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];             /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */
extern char g_pedia_win;              /* SEG20:0x387c, declared */
extern MainWin g_main_win;             /* SEG20:0x5ca */

extern void set_europe(int n);
extern void popup_say(int slot, int word);

extern void force_mood(int mood);
extern void PediaWin_ShowModal(void *pedia, int category, int article, void *p);
extern void main_screen_refresh(void);
extern void set_building(int bit, int on);
extern void set_colony(int colony);
extern void set_tribe(int tribe);
extern void set_village(int s);
extern void piss_off(int tribe, int nation, int delta, int unused);

extern int get_specialty(int u);
extern void set_specialty(int i, int s);
extern void DrawMap(int full);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void show_fathers(int nation, int father);

extern void set_father(int nation, int father, int on);
extern void freedom_name(int nation, char *buf);

extern void coronado_colony(int colony, int nation);

void give_me_liberty(int nation, int father, int unused1)
{
    int save_colony;
    int alarm;
    int spec;
    int colony;
    char buf[0x50];
    register int i;
    register int j;

    save_colony = g_cur_colony_idx;
    set_europe(nation);
    if (father >= 0) {
        set_father(nation, father, 1);
        popup_say(1, g_fathers[father].name);
        if ((int)g_game.father_owner[father] < 0)
            g_game.father_owner[father] = (char)nation;
    } else {
        freedom_name(nation, buf);
        set_name_slot_1020_a94a(1, buf);
    }
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN && father >= 0) {
        popup_say(0, g_fathers[father].name);
        popup_say(1, get_adjective(nation));
        force_mood(3);
        show_text_dialog_1020_aa02("FREEDOM");
        show_fathers(nation, father);
        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 5, father, (EventWin *)&g_main_win);
        main_screen_refresh();
    }
    g_cur_nation_rec->fathers++;
    g_cur_nation_rec->next_father = -1;
    if (father == FATHER_JAKOB_FUGGER)
        g_cur_nation_rec->boycott = 0;
    if (father == FATHER_LA_SALLE) {
        for (i = 0; g_game.colony_count > i; i++) {
            set_colony(i);
            if ((char)g_cur_colony_rec->nation != (char)nation)
                continue;
            if ((int)g_cur_colony_rec->pop < 3)
                continue;
            set_building(BUILDING_STOCKADE, 1);
        }
    }
    if (father == FATHER_JOHN_PAUL_JONES) {
        j = create_unit(father == FATHER_JOHN_PAUL_JONES ? 0x11 : 0xf, nation,
                                  nation - 0x18, nation - 0x18);
        if (j >= 0) {
            g_units[j].orders = ORDERS_NONE;
            g_units[j].dest_x = g_nations[nation].start_x;
            g_units[j].dest_y = g_nations[nation].start_y;
            g_units[j].counter = 0;
        }
    }
    if (father == FATHER_POCAHONTAS) {
        for (i = 0; i < 8; i++) {
            set_tribe(i);
            alarm = 0 - g_cur_tribe_rec->anger[nation];
            if (alarm < 0)
                piss_off(i, nation, alarm, 0);
        }
        for (i = 0; g_game.settlement_count > i; i++) {
            set_village(i);
            g_cur_settlement_rec->anger[nation] = 0;
        }
    }
    if (father == FATHER_SIMON_BOLIVAR && nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        g_game.rebel_sentiment += 0x14;
        g_game.rebel_sentiment = min_int_1020_a8ad(g_game.rebel_sentiment, 100);
    }
    if (father == FATHER_JEAN_DE_BREBEUF) {
        for (i = 0; g_game.settlement_count > i; i++) {
            set_village(i);
            if ((int)g_cur_settlement_rec->mission >= 0
                && ((int)g_cur_settlement_rec->mission & MISSION_NATION_MASK) == nation)
                g_cur_settlement_rec->mission |= MISSION_EXPERT;
        }
    }
    if (father == FATHER_BARTOLOME_DE_LAS_CASAS) {
        for (j = 0; g_game.unit_count > j; j++) {
            if ((char)(g_units[j].flags & UNIT_NATION_MASK) != (char)nation)
                continue;
            if (g_units[j].type == UNIT_COLONISTS && (int)g_units[j].spec == JOB_CONVERT)
                g_units[j].spec = JOB_COLONIST_ALT;
        }
        for (i = 0; g_game.colony_count > i; i++) {
            set_colony(i);
            if ((char)g_cur_colony_rec->nation != (char)nation)
                continue;
            for (j = 0; (int)g_cur_colony_rec->pop > j; j++) {
                spec = get_specialty(j);
                if (spec == JOB_CONVERT)
                    set_specialty(j, JOB_COLONIST_ALT);
            }
        }
    }
    if (father == FATHER_FRANCISCO_DE_CORONADO) {
        for (colony = 0; g_game.colony_count > colony; colony++) {
            set_colony(colony);
            coronado_colony(colony, nation);
        }
    }
    if (father == FATHER_WILLIAM_BREWSTER) {
        for (i = 0; i < 3; i++) {
            if (g_nations[nation].recruit[i] == 0x19 || g_nations[nation].recruit[i] == 0x1a)
                g_nations[nation].recruit[i] = 0x1c;
        }
    }
    set_colony(save_colony);
    DrawMap(1);
}
