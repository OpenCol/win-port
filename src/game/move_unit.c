/* PORT: this file's own declarations of ColChild_ActivateAnyMessage, MapWin_SlideIcon, combat_fight removed: the header declares them from the definition (W1 step 3). */
/* move_unit: candidate for 1020:cfb8 (4095 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3
 * symbol: move_unit
 * name-source: sibling-build
 *
 * A UNIT GOES TO A SQUARE -- the one routine that decides whether a step is a move or an attack,
 * whether it is allowed, whether it succeeds, and what everyone else thinks of it afterwards. It
 * takes the unit and the destination and returns nothing; what it leaves behind is the unit moved
 * (or not), the movement points spent, and a pile of diplomatic state.
 *
 * WHAT A STEP COSTS is three times the terrain's +0xa, except that a road at both ends, or a river
 * at both ends with the step on an axis, makes it 1, and a step into any owned square caps it at 3.
 * The square's owner comes from `is_city`, or from the defender's own nation when a unit is
 * standing there; a different owner makes the step an ATTACK.
 *
 * AN ATTACK IS REFUSED for several reasons, each with its own message: an AI or native unit with fewer than three
 * movement points left (it just loses its turn; a human's attack is not refused for this), a unit whose type has no attack value
 * (CANNOTATTACK for a human -- ships fail silently), a ship attacking anywhere but the two ocean
 * terrains 0x19 and 0x1a, or a land unit attacking INTO them. After the revolution the rebelling
 * nation cannot attack another European nation at all.
 *
 * ATTACKING NATIVES asks WHACKINDIANS first when the tribe is not already angry (tension under 0x4b)
 * and the treaty bit 4 is unset, and a No stops the move. The tribe's alarm then rises by
 * `difficulty + 5`, doubled when the square is a settlement and tripled again when that settlement
 * has bit 4, and the settlement's own per-nation grudge at +0xa goes up by 0x100.
 *
 * ATTACKING EUROPEANS is the long branch. A PRIVATEER is the special case -- attacking one changes no
 * relation at all (an empty arm); when one attacks, the victim never learns who it was, so all that happens is that bit 0x80 is set in the
 * relation byte, and on a `difficulty + 1` in a hundred the nation guesses -- bit 8 if it is the
 * stronger fleet, bit 2 if it is not. Otherwise HAVETREATY asks before breaking a treaty (anything
 * but answer 2 stops the move), a nation that has already been warned once is simply blocked, SNEAK
 * and CANCELPEACE are shown, and the peace bit 0x40 is cleared on both sides.
 *
 * A MOVE THAT IS NOT AN ATTACK spends the points, and a step that crosses the shoreline with no colony
 * at either end spends the unit's WHOLE allowance. Then, if the unit has fewer points than the
 * step costs and it has already moved this turn, it gets one roll: `rand_range(1, cost) > moves`
 * ends the turn with nothing happening.
 *
 * THE MOVE ITSELF relinks the unit (and everything stacked on it) into the new square, puts the
 * units stacked behind it on sentry (and, boarding a ship at sea, sentries the stack and wakes the ship), and slides the icon across the map when the square
 * is visible. A ship or a wagon train entering a colony has its moves used up by `end_unit_turn` (a human's wagon train also plays WGNSTOP), wakes the units stacked with it, and posts the
 * "arrives at" line into the map window's message. Landfall, exploration, a lost city rumour (`lost_city_at`,
 * `lost_city_exploration`), first contact (`treaty_encounters`) and a treasure train reaching a colony with an agent all hang off the tail.
 *
 * THREE THINGS THE SHIPPED CODE DOES FOR NOTHING: a local set to `x == 0x34 && y == 0xc` that is
 * never read, another set to 1 and never read, and an `if` on whether the unit is a soldier,
 * artillery or dragoon with an EMPTY body -- the compare is still emitted. Several conditions also
 * test `nation < 4` twice in a row.
 *
 * TWO NATION GLOBALS are in play and they are not the same: SEG20:0x7798 is the nation the map is
 * drawn for (called `g_game.view_nation` here) and SEG20:0x779a is the human player's own nation.
 *
 * WHAT THE BYTES SETTLED: `foreign_city`, `explore_square`,
 * `has_father`, `free_galleon2` and `combat_fight` are all in this segment
 * but reached through the five-byte NOP form, so they are EXTERN; `min_int_1020_e715` and
 * `show_text_dialog_1020_e73b` use the four-byte form and so are in-file stubs (rule 47). And the
 * nation must NOT be declared `register`: an explicit `register` takes SI away from the unit
 * parameter, which the shipped keeps there.
 *
 * RENAMED 2026-09-26 from unit_move_to_square: the Mac build's move_unit(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee AddToMessage; callee CancelMessage; callee explore_square; callee terrain_at), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

#include "fathers.h"
#include "settle.h"
#include "control.h"
#include "gameflag.h"
#include "terrain.h"
#include "units.h"
#include "nations.h"
#include "sounds.h"
#include "misc.h"

#include "w16types.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];               /* SEG20:0xc2f4 */

#include "classes.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern unsigned char feature_get(int x, int y);
extern unsigned char map_get(int x, int y);
extern int is_city(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int is_village(int x, int y);
extern int tribe_village(int u, int x, int y);

extern unsigned char move_rate(int u);
extern void end_unit_turn(int u);
extern void set_tribe(int tribe);
extern int pissed(int tribe, int i);
extern int get_treaty(int nation_a, int nation_b);
extern int get_name(int nation);
extern void popup_say(int slot, int word);
extern int popm(char *name, int n);
extern int treaty_on(int nation, int other, int bits);
extern int find_village(int x, int y);
extern void set_village(int s);
extern void piss_off(int tribe, int nation, int delta, int unused);

extern void force_mood(int mood);
extern int treaty_off(int nation, int other, int mask);
extern int terrain_is_ocean(int x, int y);
extern int is_colony(int x, int y);
extern void random_activate(int seed);
extern char owner_of(int x, int y);
extern int get_spotting(int x, int y, int nation);
extern void additional_spotting(int u, int bits);
extern int in_view(int x, int y, int nation);
extern void spot_stack(int u, int nation);
extern void stack_units(int u);
extern int master_unit_in_stack(int u);
extern int next_unit(int u);
extern int any_ship_in_stack(int u);
extern void issue_stack_orders(int u, int orders);
extern int unit_is_active(int u);
extern void unit_activate(int u);
extern void sound_play(int n);
extern int top_unit(int u);
extern void ColChild_CancelMessage(ColChild *child, int a);
extern void ColChild_AddToMessage(ColChild *child, int word);
extern void ColChild_AddStringToMessage(ColChild *message, char *s);
extern int lost_city_at(int x, int y);
extern void unspot_stack(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern void possibly_spot_stack(int u);
extern void DrawMapRegion(int x, int y, int w, int h, int flag);
extern void lost_city_exploration(int u, int x, int y);
extern void colony_screen(int c);
extern int foreign_city(int u, int colony);

extern int zoc_4(int x, int y, int nation);
extern void treaty_encounters(int u, int x, int y);
extern int has_father(int nation, int bit);
extern void free_galleon2(int u, int nation);

/* same object: 4-byte `push cs; call ` (rule 47) */

void move_unit(register int u, int x, int y)
{
    int u2 = -1;
    int ox;
    int oy;
    int unused = 1;
    int unit;
    int terr;
    int cost;
    int head;
    int nxt;
    int col;
    int col2;
    int redraw;
    int anim;
    int slide;
    int tnation;
    int rumour;
    int moves;
    int tn;
    int tu;
    int hostile;
    int fresh;
    int utype;
    int armed;
    int anger;
    int tension;
    int bits;
    int village;
    int count;
    int pending = -1;
    int done = 0;
    int corner = 0;
    int nation;

    corner = (g_units[u].x == 0x34 && g_units[u].y == 0xc);
    count = g_game.unit_count;
    ox = g_units[u].x;
    oy = g_units[u].y;
    terr = terrain_at(x, y);
    cost = g_terrain_rows[terr].movement * 3;
    if ((feature_get(ox, oy) & (MAP1_SETTLEMENT | MAP1_ROAD)) != 0 && (feature_get(x, y) & (MAP1_SETTLEMENT | MAP1_ROAD)) != 0)
        cost = 1;
    if ((map_get(ox, oy) & MAP0_RIVER) != 0 && (map_get(x, y) & MAP0_RIVER) != 0 &&
        (ox == x || oy == y))
        cost = 1;
    nation = NATION(g_units[u]);
    tn = is_city(x, y);
    if (tn >= 0)
        cost = min_int_1020_e715(cost, 3);
    tu = top_unit_in_square(x, y);
    if (tu >= 0)
        tn = NATION(g_units[tu]);
    hostile = (tn >= 0 && tn != nation);
    if (nation < NATION_EUROPEAN_COUNT && is_village(x, y) >= 0 &&
        tribe_village(u, x, y) != 0)
        goto out;
    if (hostile != 0 && nation < NATION_EUROPEAN_COUNT && tn < NATION_EUROPEAN_COUNT) {
        col2 = find_colony(x, y);
        if (col2 >= 0) {
            if (foreign_city(u, col2) != 0)
                goto out;
            if (g_game.unit_count != count)
                goto out;
        }
    }
    moves = move_rate(u) - g_units[u].moves_used;
    if (hostile != 0) {
        if (moves < 3 && (nation >= NATION_EUROPEAN_COUNT ||
                          g_nation_text[nation].control != CONTROL_HUMAN)) {
            end_unit_turn(u);
            goto out;
        }
        if (g_unit_types[g_units[u].type].attack == 0) {
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                goto out;
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
                show_text_dialog_1020_e73b("CANNOTATTACK");
            goto out;
        }
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
            if (terr != TERRAIN_OCEAN && terr != TERRAIN_SEA_LANE) {
                end_unit_turn(u);
                goto out;
            }
        } else if (terr == TERRAIN_OCEAN || terr == TERRAIN_SEA_LANE) {
            end_unit_turn(u);
            goto out;
        }
        if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0 && g_game.mother_country_idx == nation &&
            tn < NATION_EUROPEAN_COUNT &&
            (tn >= NATION_EUROPEAN_COUNT || g_nation_text[tn].control != CONTROL_HUMAN))
            goto out;
        if (tn >= NATION_EUROPEAN_COUNT) {
            set_tribe(tn - NATION_EUROPEAN_COUNT);
            if (pissed(g_cur_tribe, NATION(g_units[u])) < 0x4b &&
                nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN &&
                (get_treaty(nation, tn) & 4) == 0) {
                popup_say(0, get_name(tn));
                if (popm("WHACKINDIANS", 1) != 1)
                    goto out;
                treaty_on(nation, tn, 4);
            }
            anger = g_game.difficulty + 5;
            village = find_village(x, y);
            if (village >= 0) {
                set_village(village);
                anger = anger * 2;
                g_cur_settlement_rec->anger[NATION(g_units[u])] += 0x100;
                if ((g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL) != 0)
                    anger = anger * 3;
            }
            piss_off(g_cur_tribe, NATION(g_units[u]), anger, 0);
        } else if (nation < NATION_EUROPEAN_COUNT &&
                   g_units[tu].type == UNIT_PRIVATEER) {
            ;
        } else if (nation < NATION_EUROPEAN_COUNT &&
                   g_units[u].type == UNIT_PRIVATEER) {
            g_nations[tn].rel[nation] |= 0x80;
            if (rand_range(0, 100) < g_game.difficulty + 1) {
                if (g_military_power[tn] >= g_military_power[nation])
                    g_nations[tn].rel[nation] |= 8;
                else
                    g_nations[tn].rel[nation] |= 2;
            }
        } else if (nation < NATION_EUROPEAN_COUNT) {
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN &&
                (get_treaty(nation, tn) & 0x40) != 0) {
                popup_say(0, get_name(tn));
                if (popm("HAVETREATY", 1) != 2)
                    goto out;
            }
            if ((nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN) &&
                tn < NATION_EUROPEAN_COUNT &&
                (get_treaty(nation, tn) & 0x40) != 0 &&
                g_nations[nation].timer[tn] > 0)
                goto out;
            if (tn < NATION_EUROPEAN_COUNT && g_nation_text[tn].control == CONTROL_HUMAN &&
                ((get_treaty(tn, nation) & 0x40) != 0 ||
                 (get_treaty(nation, tn) & 1) != 0)) {
                popup_say(0, get_name(nation));
                force_mood(4);
                show_text_dialog_1020_e73b("SNEAK");
            }
            if ((get_treaty(nation, tn) & 0x40) != 0 ||
                (get_treaty(tn, nation) & 0x40) != 0) {
                if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                    popup_say(0, get_name(tn));
                    popup_say(1, get_name(nation));
                    force_mood(4);
                    popm("CANCELPEACE", 1);
                }
                if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN &&
                    (get_treaty(tn, nation) & 0x40) != 0)
                    g_nations[tn].rel[nation] |= 2;
                treaty_off(nation, tn, 0x40);
            }
            g_nations[nation].rel[tn] &= 0xfe;
        }
    }
    fresh = !g_units[u].moves_used;
    if (hostile == 0) {
        g_units[u].moves_used = g_units[u].moves_used + cost;
        if (terrain_is_ocean(ox, oy) != terrain_is_ocean(x, y) && is_colony(ox, oy) < 0 &&
            is_colony(x, y) < 0)
            g_units[u].moves_used = move_rate(u);
    }
    if (moves < cost && fresh == 0) {
        random_activate(g_random_seed);
        if (hostile == 0 && rand_range(1, cost) > moves) {
            done = 1;
            goto out;
        }
    }
    if (nation < NATION_EUROPEAN_COUNT) {
        tn = owner_of(x, y);
        tension = pissed(tn - NATION_EUROPEAN_COUNT, nation);
        if (tn >= NATION_EUROPEAN_COUNT && tension < 0x4b) {
            set_tribe(tn - NATION_EUROPEAN_COUNT);
            utype = g_units[u].type;
            armed = (utype == UNIT_SOLDIERS || utype == UNIT_ARTILLERY ||
                     utype == UNIT_DRAGOONS);
            if (armed != 0)
                ;
        }
    }
    redraw = g_game.reveal_all;
    if (NATION(g_units[u]) != (unsigned char)g_game.view_nation) {
        bits = get_spotting(x, y, NATION(g_units[u]));
        additional_spotting(u, bits);
        if ((UNIT_SEEN_BY(g_game.view_nation) & g_units[u].flags) != 0 ||
            in_view(x, y, g_game.view_nation) != 0)
            redraw = 1;
    }
    anim = redraw;
    if (hostile == 0) {
        if (NATION(g_units[u]) >= NATION_EUROPEAN_COUNT) {
            if ((HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_SHOW_INDIAN_MOVES) == 0)
                anim = 0;
        } else if ((HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_SHOW_FOREIGN_MOVES) == 0)
            anim = 0;
    }
    if (NATION(g_units[u]) == (unsigned char)g_game.view_nation) {
        redraw = 1;
        anim = 1;
    }
    if (hostile != 0) {
        if ((nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) ||
            (tn < NATION_EUROPEAN_COUNT && g_nation_text[tn].control == CONTROL_HUMAN))
            redraw = 1;
        combat_fight(u, x, y, redraw, 1);
        goto out;
    }
    if (redraw != 0)
        spot_stack(u, g_game.view_nation);
    stack_units(u);
    if (anim != 0) {
        slide = 0xc0;
        if (NATION(g_units[u]) != (unsigned char)g_game.view_nation)
            slide |= 0x20;
        MapWin_SlideIcon(&g_map_window, u, slide, -1, ox, oy, x, y);
    }
    head = master_unit_in_stack(u);
    nxt = next_unit(head);
    while (nxt >= 0) {
        g_units[nxt].orders = ORDERS_SENTRY;
        nxt = next_unit(nxt);
    }
    unit = top_unit_in_square(x, y);
    unit = master_unit_in_stack(unit);
    if (unit >= 0 && terrain_is_ocean(x, y) != 0 && any_ship_in_stack(head) == 0) {
        if (g_units[unit].orders == ORDERS_SENTRY)
            g_units[unit].orders = ORDERS_NONE;
        issue_stack_orders(head, ORDERS_SENTRY);
        if (unit_is_active(unit) != 0)
            unit_activate(unit);
    }
    if (any_ship_in_stack(u) != 0 || g_units[u].type == UNIT_WAGON_TRAIN) {
        col = find_colony(x, y);
        if (col >= 0) {
            if (g_units[u].type == UNIT_WAGON_TRAIN &&
                NATION(g_units[u]) < NATION_EUROPEAN_COUNT &&
                g_nation_text[NATION(g_units[u])].control == CONTROL_HUMAN)
                sound_play(SOUND_WGNSTOP);
            end_unit_turn(u);
            g_units[u].counter = 0;
            unit = u;
            u = top_unit(u);
            while (u >= 0) {
                if (g_units[u].orders == ORDERS_SENTRY)
                    g_units[u].orders = ORDERS_NONE;
                u = next_unit(u);
            }
            u = unit;
            if (NATION(g_units[u]) == (unsigned char)g_game.view_nation) {
                ColChild_CancelMessage((ColChild *)&g_map_window, 1);
                ColChild_AddToMessage((ColChild *)&g_map_window, g_unit_types[g_units[u].type].name);
                if (g_units[u].type == UNIT_WAGON_TRAIN)
                    ColChild_AddToMessage((ColChild *)&g_map_window, g_misc_words[MISC_ARRIVES_IN]);
                else
                    ColChild_AddToMessage((ColChild *)&g_map_window, g_misc_words[MISC_DOCKS_AT]);
                ColChild_AddStringToMessage((ColChild *)&g_map_window, g_colonies[col].name);
                ColChild_ActivateAnyMessage((ColChild *)((char *)&g_map_window), 1, 0x78L);
            }
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST &&
                (g_game.game_flags & GAMEFLAG_TUTORIAL) != 0 &&
                NATION(g_units[u]) == (unsigned char)g_game.player_nation &&
                (HI_BYTE(g_game.hint_flags) & 0x80) == 0)
                pending = col;
        }
    }
    rumour = lost_city_at(x, y);
    bits = get_spotting(x, y, NATION(g_units[u]));
    tnation = owner_of(x, y);
    unspot_stack(u);
    relocate_stacked_units(u, x, y);
    if (tnation >= 0)
        spot_stack(u, tnation);
    possibly_spot_stack(u);
    additional_spotting(u, bits);
    if (terrain_is_ocean(ox, oy) != 0 && terrain_is_ocean(x, y) == 0 && is_colony(x, y) < 0) {
        bits = g_units[u].flags & 0xf0;
        u2 = top_unit_in_square(ox, oy);
        if (u2 >= 0)
            additional_spotting(u2, bits);
    }
    explore_square(u);
    if (redraw != 0)
        DrawMapRegion(x - 3, y - 3, 7, 7, 1);
    if (rumour != 0 && nation < NATION_EUROPEAN_COUNT)
        lost_city_exploration(u, x, y);
    if (g_game.unit_count != count)
        goto out;
    if (zoc_4(x, y, nation) != 0)
        treaty_encounters(u, x, y);
    if (g_units[u].type == UNIT_TREASURE_TRAIN && nation < NATION_EUROPEAN_COUNT &&
        g_nation_text[nation].control == CONTROL_HUMAN && is_colony(x, y) >= 0) {
        if (g_unit_tally[nation][0xf] == 0 || (g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0 ||
            has_father(nation, FATHER_HERNAN_CORTES) != 0) {
            col2 = find_colony(x, y);
            if ((g_colonies[col2].status & 0x40) != 0)
                free_galleon2(u, nation);
        }
    }
    if (pending > 0)
        colony_screen(pending);
    done = 1;
out:
    if (g_game.unit_count != count)
        return;
    if (done == 0) {
        g_units[u].orders = ORDERS_NONE;
        if (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN) {
            g_units[u].counter++;
            if (g_units[u].counter >= 0x14) {
                g_units[u].counter = 0;
                end_unit_turn(u);
            }
        }
    }
}
