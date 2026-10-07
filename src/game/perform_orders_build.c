/* perform_orders_build: candidate for 1028:ab1f (798 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * BUILD COLONY -- `foreign_unit`'s order 7 (ORDERS_BUILD_COLONY), the sibling of
 * `perform_orders_road` (order 9). The unit's work counter and orders are cleared,
 * the view scrolls to the square if the player is watching this nation, a name is proposed,
 * `create_colony` does the work, and the new colony is given five buildings (the fur trader's,
 * tobacconist's, weaver's, rum distiller's and blacksmith's houses).
 *
 * THE HUMAN PLAYER AND THE AI TAKE THE NAME BY DIFFERENT ROUTES. Both get a proposal from
 * `get_colony_name` into an 80-byte stack buffer. The human is then shown the `COLONY`
 * prompt with that proposal as the default -- and a non-zero return (cancel) abandons the
 * whole mission, leaving the unit with its orders already cleared. What the human typed
 * lands in the shared name buffer at SEG31:0xa4, so the copy into the colony record reads
 * from THERE; only the AI copies the stack buffer. That is why the same `STRCPY` appears
 * twice with the two arms of the same test around it.
 *
 * `g_popup_dude` is set to 5 across the prompt and back to -1 after, which is the same
 * save-restore the other TEXT prompts do.
 *
 * SEG20:0x7790 IS A TURN COUNTER, NOT THE TREASURY. `GameBrains_DoEndofTurn` does `INC WORD
 * [0x7790]` once a turn, and new-game setup writes 0 there beside the year 1492 at 0x778c.
 * Here it is stamped into +0x46 of the founding nation's 316-byte record -- the turn the
 * nation last founded a colony (it is overwritten on every founding). Five earlier headers call it `g_treasury`; this one and the
 * sweep beside it call it `g_game.turn_counter`.
 *
 * THREE LOCALS ARE WRITTEN AND NEVER READ: the map-plane-0 pointer, the terrain, and the
 * word set to 3 and then 1. They cost eleven instructions in the shipped code and have to
 * be spelled out or the frame is the wrong size.
 *
 * `x` IS THE PROMOTED LOCAL AND MUST BE DECLARED BEFORE `y`. Both are plain `int`s used
 * five times each; the allocator promotes the FIRST eligible one in declaration order, so
 * declaring `y` first puts `y` in DI and pushes `x` onto the stack -- the shipped has it
 * the other way round (rule 53's fourth tier). The parameter is `register int u` and takes
 * SI, which a promoted local never outranks.
 *
 * Both early exits are the two-jump form and so are `goto`s, not `return`s: one epilogue
 * for the whole function (rules 12 and 44). `get_colony_name` is -called with the
 * 4-byte `PUSH CS` form and so is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from mission_found_colony: the Mac build's perform_orders_build(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee Show; callee create_colony; callee terrain_at), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "building.h"
#include "units.h"
#include "woodcut.h"
#include "nations.h"
#include "sounds.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];             /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

/* PORT: this file's own ColonyWin removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"

extern unsigned char *map_loc(int x, int y);

extern int possibly_center(int x1, int y1, int x2, int y2, int m);
extern int popup_ask(void *ctx, void *name, int max_len, void *dflt);
extern void end_unit_turn(int u);
extern int create_colony(int nation, int x, int y, int unit);

extern void set_building(int item, int on);
extern void DrawMap(int flag);
extern void sound_play(int id);

extern void ColonyWin_Show(void *w, int colony);

/* same object: 4-byte `push cs; call ` */
extern void get_colony_name(int nation, char *out);

void perform_orders_build(register int u)
{
    unsigned char *plane0;
    int x;
    int y;
    int terr;
    int col;
    int step;
    char name[0x50];

    step = 3;
    x = g_units[u].x;
    y = g_units[u].y;
    plane0 = map_loc(x, y);
    terr = terrain_at(x, y);
    step = 1;
    g_units[u].counter = 0;
    g_units[u].orders = ORDERS_NONE;
    if (g_game.turn_nation == g_game.view_nation
        || (g_game.reveal_all != 0
            && (g_game.game_flags & (g_game.turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000)) != 0))
        possibly_center(x, y, x, y, 1);
    get_colony_name(g_game.turn_nation, name);
    if (g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN) {
        g_popup_dude = 5;
        if (popup_ask(g_game_txt, "COLONY", 0x17, name) != 0)
            goto done;
        g_popup_dude = -1;
    }
    end_unit_turn(u);
    g_nations[g_game.turn_nation].founded = g_game.turn_counter;
    col = create_colony(g_game.turn_nation, x, y, u);
    if (col < 0)
        goto done;
    if (g_game.turn_nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_game.turn_nation].control != CONTROL_HUMAN)
        STRCPY(g_cur_colony_rec->name, name);
    set_building(BUILDING_FUR_TRADERS_HOUSE, 1);
    set_building(BUILDING_TOBACCONISTS_HOUSE, 1);
    set_building(BUILDING_WEAVERS_HOUSE, 1);
    set_building(BUILDING_RUM_DISTILLERS_HOUSE, 1);
    set_building(BUILDING_BLACKSMITHS_HOUSE, 1);
    if (g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN)
        STRCPY(g_cur_colony_rec->name, g_name_slots[0]);
    if (g_game.turn_nation == g_game.view_nation
        || (g_game.reveal_all != 0
            && (g_game.game_flags & (g_game.turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000)) != 0))
        DrawMap(1);
    if (g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN) {
        sound_play(SOUND_HAMMERS);
        woodcut(WOODCUT_BUILDING_A_COLONY);
        g_colony_mode = 0;
        g_colony_c24 = 0;
        ColonyWin_Show(&g_colony_window, col);
    }
done:
    ;
}
