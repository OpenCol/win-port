/* PORT: this file's own declarations of get_defense removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of MapWin_SlideIcon, delete_colony, special_kill removed: the header declares them from the definition (W1 step 3). */
/* combat_fight: candidate for 1020:4460 (8698 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: combat_fight
 * name-source: sibling-build
 * size-source: 8698, not the census 8690. The last eight bytes are the four-entry jump table the
 * treasure switch reads through `CS:[BX+0x6652]`; `sub_1020_6684` starts after them.
 *
 * ONE ATTACK, FROM THE ODDS TO THE AFTERMATH. Every fight in the game goes through here: unit
 * against unit, unit against colony, unit against native village, and the same code computes the
 * odds bar the player sees before committing. `real == 0` means "just tell me the numbers" -- the
 * function builds everything up to the strength comparison, tears down whatever it created, and
 * returns `attack * 8 / (defence + 1)` without rolling a die.
 *
 * AN UNDEFENDED SETTLEMENT STILL FIGHTS. When there is no unit on the square the function MAKES a
 * defender: a colony picks a random colonist and gives it that colonist's portrait, or 0x4b and one more
 * point of defence when the owner has Paul Revere and fifty or more muskets stored; a village
 * uses type 0x13, one type up when the tribe has muskets and two up when it has 25 or more
 * horses, and copies that type's name over the fake unit's type record's. The temporary unit is released again on every exit
 * path, including the odds-only one.
 *
 * THE STRENGTH CHAIN is where the game's combat rules actually live, and every modifier sets a bit
 * in the four words at DGROUP:0x4bda..0x4be0 so the odds window can list them:
 *   an attacker with movement left under three fights at `attack * moves_left / 3`, and below three points of movement the
 *   player is warned with HALF and may call the attack off;
 *   both sides get `4 - difficulty` added when they are a human-played European nation;
 *   a strong attacker (attack above 1) halves a weak defender's (defence below 2) whole defence, ships excepted;
 *   ARTILLERY IN THE OPEN loses three quarters of its strength, on either side;
 *   artillery defending a settlement against natives doubles instead;
 *   the Spanish (nation 2) gain half again attacking natives in a settlement;
 *   and during the War of Independence an attack on a colony gains a percent bonus: the colony's
 *   rebel percentage for a rebel attacker, 100 less it (the Tory share) for the Crown.
 * Then come the handicaps: on the two easiest levels an attack on a human player's colony before turn 80 is halved
 * (Explorer) or cut by a quarter (Discoverer) and, on Discoverer against an undefended colony, reduced to nothing; on Discoverer a human attacker is doubled.
 *
 * SHIPS EVADE. A ship with the higher attack rating attacking another rolls the two `blockade_speed` figures against each other,
 * and on a loss the attacker is simply put back where it started with EVASIVE -- no damage either
 * way. That is checked AFTER the animation, so the slide still plays.
 *
 * WHAT HAPPENS AFTERWARDS is four separate endings. A beaten unit is destroyed or demoted by
 * `special_kill`/`special_kill_stack`. A beaten colony is captured -- ownership, the colony
 * count and population totals per nation move over, the bell store drops to two thirds, the eight
 * surrounding squares change hands, and a share of the loser's treasury in proportion to the
 * colony's size follows. A beaten village shrinks by one and is razed at size one; razing a capital clears every one of the tribe's settlements' anger with that nation,
 * and a TREASURE is generated whose size comes from a four-way switch on the tribe's level --
 * camps and villages give tens, cities and capitals give hundreds -- multiplied again for a capital
 * (the flag was named `burned` until 2026-09-27), for Cortes and for the Spanish. And a native attack that fails a colony becomes
 * a raid through `resolve_raid` instead.
 *
 * PROMOTION runs both ways: the survivor is passed to `try_to_improve`, and a brave that beats a
 * dragoon or a soldier upgrades itself by two types (mounted) or one (armed), which is how natives
 * acquire horses and muskets from the battlefield.
 *
 * WHAT THE BYTES SETTLED, five spellings.
 *   The battle-sound chain is reached by a `goto`, not by wrapping it in `if (woodcut == 0)`: the
 *   shipped jumps with the short-JCC-over-JMP form that only an explicit jump produces (rule 12),
 *   and the structured `if` gives a long conditional four bytes further along.
 *   Three nation comparisons are BYTE compares, so `(unsigned char)(unit->nation & 0xf)` keeps the
 *   AND in AL where the plain expression promotes first (rule 83).
 *   `(int)colony->pop > 1` needs its cast, because a signed-char field compares as a byte without
 *   one.
 *   The `default:` label of the treasure switch is NOT in the source -- writing one adds a second
 *   `JMP +0` at the join, because the range check already jumps there.
 *   And two `||` chains had to be read off the jump targets rather than from the game's rules: the
 *   highlight flag is set when the square is unowned OR THE ATTACKER WON, and the CAPTURED message
 *   is chosen when EITHER side is human-played, not when one is and the other is not.
 *
 * RENAMED 2026-09-26 from resolve_attack: the Mac build's combat_fight(short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (callee SlideIcon; callee best_defender; callee get_name; callee on_map; callee resolve_raid), the runner-up gets 2, and the arity agrees (5).
 */

#include "colonization.h"

#include "difficul.h"
#include "settle.h"
#include "fathers.h"
#include "jobs.h"
#include "nations.h"
#include "control.h"
#include "terrain.h"
#include "woodcut.h"
#include "gameflag.h"
#include "sounds.h"
#include "units.h"
#include "misc.h"
#include "goods.h"
#include "records.h"

extern Nation g_nations[];               /* SEG20:0xc2f4, 0x13c bytes each */

extern GameState g_game;   /* SEG20:0x7782 */

/* PORT: this file's own MapWin removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* MapWin : ColChild, the base as its bytes: 0x788 */

extern unsigned char move_rate(int u);
extern void end_unit_turn(int u);
extern int top_unit_in_square(int x, int y);
extern int is_city(int x, int y);

extern int find_village(int x, int y);
extern void set_colony(int colony);

extern int get_specialty(int u);
extern int ColonistIcon(int spec);
extern void set_village(int s);
extern int popm(char *msg, int n);   /* 1050:64d1 */
extern int colony_rebel(void);
extern int terrain_is_ocean(int x, int y);
extern int is_colony(int x, int y);
extern void stack_units(int u);
extern void move_to_top(int u);
extern void sound_play(int n);

extern void force_map(int x, int y);
extern int blockade_speed(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern void DrawMapRegion(int x, int y, int w, int h, int flag);

extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern void move_to_bottom(int u);
extern void delete_colonist(int colonist);
extern void set_tribe(int tribe);
extern void delete_village(int s);

extern int bottom_unit(int u);
extern void kill_stack(int u);
extern void DrawMap(int flag);
extern void ShowMapRegion(int x, int y, int w, int h);
extern int get_name(int nation);
extern int find_close_colony(int x, int y, int nation, int land);
extern int job_name(int spec);
extern void force_mood(int mood);
extern void owner_set(int x, int y, int n);
extern int is_anything(int x, int y);
extern int next_unit(int u);
extern void colony_screen(int c);
extern void force_music(int track);

extern int get_treaty(int nation_a, int nation_b);
extern void piss_off(int tribe, int nation, int delta, int f);
extern int pissed(int tribe, int n);
extern int popi(char *name, int sel);

extern void feature_set(int x, int y, int bit, int on);
extern void ShowMap(void);
extern void combat_analysis(int u, int u1, int ux, int uy, int x, int y,
                                      int an, int dn, int colony, int settlement, int att,
                                      int def, int roll);
extern int make_fake_unit(int portrait, int x, int y, int owner, int strength);
extern void blow_fake_unit(void);
extern int has_father(int nation, int bit);
extern int get_combat(int u, int attacking);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void free_galleon(int n);

extern "C" void resolve_raid(int native, int colony, int settlement, int force, int t);

extern "C" void try_to_improve(int u, int base, int bonus);
extern "C" void special_kill_stack(int u, int winner, int show, int x, int y);

int combat_fight(register int u, int x, int y, int show, int real)
{
    register int unit;
    int att;
    int def;
    int head;
    int defender;
    int won;
    int colony_taken;
    int destroyed;
    int made_defender;
    int slide;
    int ux;
    int uy;
    int x0;
    int y0;
    int w;
    int h;
    int colony;
    int village;
    int victim;
    int victim_spec;
    int sq_nation;
    int anation;
    int dnation;
    int a_f6;
    int d_f6;
    int atype;
    int dtype;
    int roll;
    int newunit;
    int a_is_ship;
    int d_is_ship;
    int r1;
    int r2;
    int moves_left;
    int partial_moves;
    int delta;
    int near_colony;
    int gift;
    int fail;
    int popsum;
    int ci;
    int kind;
    int is_capital;
    int captured;
    int ship_victory;
    int euro_vs_euro;
    int fake_defence;
    int vtype;
    int portrait;
    int chance;
    int flagA;
    int village_mission;
    int prom1;
    int prom2;
    int f6e;
    int f70;
    int f72;
    int dir;
    int nx;
    int ny;
    int f7a;
    int sol;
    int bit;
    int woodcut_show;
    int f82;
    long gold;
    char buf[0x50];

    colony_taken = 0;
    destroyed = 0;
    made_defender = 0;
    colony = -1;
    village = -1;
    partial_moves = 0;
    village_mission = -1;
    prom1 = 0;
    prom2 = 0;
    f6e = 0;
    f70 = 0;
    f82 = 0;
    atype = g_units[u].type;
    moves_left = move_rate(u) - g_units[u].moves_used;
    if (real != 0)
        g_units[u].moves_used = g_units[u].moves_used + 3;
    anation = (unsigned char)(g_units[u].flags & 0xf);
    a_f6 = g_units[u].home;
    a_is_ship = (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST);
    head = top_unit_in_square(x, y);
    defender = best_defender(head, anation);
    g_combat_flags[0] = 0;
    g_combat_flags2[0] = 0;
    g_combat_flags[1] = 0;
    g_combat_flags2[1] = 0;
    if (moves_left < 3) {
        partial_moves = moves_left;
        if (real != 0) {
            if (anation >= NATION_EUROPEAN_COUNT || g_nation_text[anation].control != CONTROL_HUMAN)
                goto out;
            set_text_arg_i_1020_a926(0, partial_moves);
            if (popm("HALF", 1) != 1)
                goto out;
        }
        if (moves_left == 2)
            g_combat_flags[0] |= 0x100;
        if (moves_left == 1)
            g_combat_flags2[0] |= 8;
    }
    if (real != 0)
        end_unit_turn(u);
    sq_nation = is_city(x, y);
    colony = find_colony(x, y);
    if (defender < 0) {
        made_defender = 1;
        if (sq_nation < 0) {
            fail = 1;
            goto vanish;
        }
        if (colony < 0) {
            village = find_village(x, y);
            if (village < 0) {
                fail = 2;
                goto vanish;
            }
        }
        if (colony >= 0) {
            set_colony(colony);
            victim = rand_range(0, g_cur_colony_rec->pop - 1);
            victim_spec = get_specialty(victim);
            fake_defence = g_unit_types[UNIT_COLONISTS].defence;
            g_combat_flags[1] |= 0x200;
            portrait = ColonistIcon(victim_spec);
            if (has_father(sq_nation, FATHER_PAUL_REVERE)
                && g_cur_colony_rec->stock[GOODS_MUSKETS] >= 0x32) {
                portrait = 0x4b;
                fake_defence++;
                g_combat_flags[1] |= 0x400;
            }
            newunit = make_fake_unit(portrait, x, y, sq_nation, fake_defence);
            g_units[newunit].spec = victim_spec;
        } else {
            set_village(village);
            vtype = 0x13;
            if (g_cur_tribe_rec->muskets != 0)
                vtype++;
            if (g_cur_tribe_rec->horses >= 0x19)
                vtype += 2;
            village_mission = g_cur_settlement_rec->mission;
            fake_defence = g_unit_types[vtype].defence;
            newunit = make_fake_unit(g_unit_types[vtype].icon, x, y,
                                                   sq_nation, fake_defence);
            g_unit_types[g_units[newunit].type].name = g_unit_types[vtype].name;
        }
        defender = newunit;
    }
    if (defender < 0) {
        fail = 3;
        goto vanish;
    }
    dnation = (unsigned char)(g_units[defender].flags & 0xf);
    dtype = g_units[defender].type;
    d_f6 = g_units[defender].home;
    def = get_defense(defender, u);
    d_is_ship = (g_units[defender].type >= UNIT_SHIP_FIRST && g_units[defender].type <= UNIT_SHIP_LAST);
    att = get_combat(u, 1);
    att = (g_terrain_defence + 4) * att >> 2;
    att = att * 3 >> 1;
    if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)
        att = 4 - g_game.difficulty + att;
    if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN)
        def = 4 - g_game.difficulty + def;
    if (partial_moves != 0)
        att = att * partial_moves / 3;
    if (a_is_ship == 0 && d_is_ship == 0 && g_unit_types[atype].attack > 1
        && g_unit_types[dtype].defence < 2)
        def = def >> 1;
    if (sq_nation < 0) {
        if (atype == UNIT_ARTILLERY
            && ((g_units[defender].orders != ORDERS_FORTIFY && g_units[defender].orders != ORDERS_FORTIFIED)
                || dnation >= NATION_EUROPEAN_COUNT)) {
            att = att >> 2;
            g_combat_flags[0] |= 0x800;
        }
        if (dtype == UNIT_ARTILLERY
            && ((g_units[defender].orders != ORDERS_FORTIFY && g_units[defender].orders != ORDERS_FORTIFIED)
                || dnation >= NATION_EUROPEAN_COUNT)) {
            g_combat_flags[1] |= 0x800;
            def = def >> 2;
        }
    } else {
        if (dtype == UNIT_ARTILLERY && anation >= NATION_EUROPEAN_COUNT) {
            g_combat_flags2[1] |= 1;
            def = def * 2;
        }
    }
    if (anation == NATION_SPAIN && dnation >= NATION_EUROPEAN_COUNT && sq_nation >= 0) {
        att += att >> 1;
        g_combat_flags[0] |= 0x1000;
    }
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && anation < NATION_EUROPEAN_COUNT) {
        if (colony >= 0) {
            if (g_game.mother_country_idx == anation || (g_game.game_flags & GAMEFLAG_INTERVENTION)) {
                g_combat_flags[0] |= 0x8000;
                att += att >> 1;
            }
            set_colony(colony);
            sol = colony_rebel();
            if (g_game.mother_country_idx == anation) {
                bit = 2;
                sol = 100 - sol;
            } else
                bit = 4;
            if (sol != 0) {
                g_combat_flags2[0] |= bit;
                att += att * sol / 100;
            }
        } else {
            if (g_game.mother_country_idx == anation && terrain_is_ocean(x, y) == 0)
                att += g_game.difficulty * att / 0x14;
        }
        if (g_game.mother_country_idx == anation && is_colony(x, y) >= 0)
            ;
    }
    if (real == 0) {
        if (made_defender != 0)
            blow_fake_unit();
        return att * 8 / (def + 1);
    }
    if (g_game.difficulty <= DIFFICULTY_EXPLORER) {
        if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 || colony < 0
            || (atype >= UNIT_SHIP_FIRST && atype <= UNIT_SHIP_LAST)) {
            if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN && g_game.turn_counter < 0x50
                && colony >= 0) {
                if (g_game.difficulty != DIFFICULTY_DISCOVERER)
                    att = att >> 1;
                else
                    att -= att >> 2;
                if (made_defender != 0 && g_game.difficulty == DIFFICULTY_DISCOVERER)
                    att = 0;
            }
            if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN
                && (anation < NATION_EUROPEAN_COUNT || g_game.turn_counter < 0x50))
                att = att >> 1;
        }
    }
    if (g_game.difficulty == DIFFICULTY_DISCOVERER && anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)
        att = att * 2;
    if (colony >= 0) {
        if (anation >= NATION_EUROPEAN_COUNT && g_nation_colony_count[dnation] == 1)
            att = 0;
        if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN
            && g_colonies[colony].pop >= g_pop_by_nation[dnation] >> 1)
            def += (4 - g_game.difficulty) << 2;
    }
    ux = g_units[u].x;
    uy = g_units[u].y;
    stack_units(u);
    move_to_top(u);
    roll = rand_range(1, att + def);
    won = (roll <= att);
    if (anation >= NATION_EUROPEAN_COUNT && dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN
        && atype == UNIT_BRAVES && dtype == UNIT_ARTILLERY) {
        won = 0;
        f82 = 1;
    }
    if (show != 0) {
        woodcut_show = 0;
        if (anation >= NATION_EUROPEAN_COUNT && dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN) {
            sound_play(atype + 0x3b);
            woodcut_show = woodcut_get(WOODCUT_INDIAN_RAID);
            woodcut(WOODCUT_INDIAN_RAID);
        }
        if ((g_game.game_flags & GAMEFLAG_COMBAT_ANALYSIS)
            && ((anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)
                || (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN)
                || g_game.reveal_all != 0)) {
            force_map(x, y);
            combat_analysis(u, defender, ux, uy, x, y, anation, dnation,
                                  g_defence_colony, g_defence_village, att, def, roll);
            woodcut_show = 0;
        }
        if (woodcut_show != 0)
            goto slide_it;
        if (a_is_ship != 0)
            sound_play(SOUND_CANNON0);
        else if (atype == UNIT_ARTILLERY || dtype == UNIT_ARTILLERY)
            sound_play(SOUND_CANNON0);
        else if (atype == UNIT_DRAGOONS || atype == UNIT_SCOUTS || atype == UNIT_CAVALRY || atype == UNIT_CONTINENTAL_CAVALRY)
            sound_play(SOUND_DRAGOONS);
        else if (g_unit_types[atype].attack > 1 || g_unit_types[dtype].attack > 1)
            sound_play(SOUND_BIGGUN);
        else
            sound_play(SOUND_LILGUN);
slide_it:
        slide = 0xc0;
        if (is_city(x, y) < 0 || won != 0)
            slide = slide | 0x10;
        if ((unsigned char)(g_units[u].flags & 0xf) != ((unsigned char)g_game.view_nation))
            slide |= 0x20;
        MapWin_SlideIcon(&g_map_window, u, slide, defender, ux, uy, x, y);
    }
    if (a_is_ship != 0 && d_is_ship != 0
        && g_unit_types[atype].attack > g_unit_types[dtype].attack) {
        r1 = blockade_speed(u);
        r2 = blockade_speed(defender);
        if (rand_range(1, r1 + r2) <= r2) {
            relocate_stacked_units(u, ux, uy);
            if (show != 0) {
                DrawMapRegion(ux - 1, uy - 1, 3, 3, 1);
                popup_say(0, get_adjective(dnation));
                popup_say(1, g_unit_types[dtype].name);
                popup_say(2, get_adjective(anation));
                popup_say(3, g_unit_types[atype].name);
                popm("EVASIVE", 0);
            }
            goto out;
        }
    }
    if (made_defender != 0)
        blow_fake_unit();
    if (won != 0) {
        move_to_bottom(u);
        if (anation >= NATION_EUROPEAN_COUNT && colony >= 0
            && ((int)g_cur_colony_rec->pop > 1 || made_defender == 0)) {
            f6e = 1;
            g_units[u].flags2 |= 0x10;
        }
        if (made_defender != 0 && colony >= 0 && anation < NATION_EUROPEAN_COUNT)
            colony_taken = 1;
        if (made_defender == 0) {
            if (show != 0) {
                if (a_is_ship != 0 || atype == UNIT_ARTILLERY)
                    sound_play(SOUND_CANNON1);
                else if (sq_nation >= 0)
                    sound_play(SOUND_BOOM2);
                else
                    sound_play(SOUND_LILGUN);
            }
            if (g_unit_types[atype].attack != 0 && (atype < UNIT_SHIP_FIRST || atype > UNIT_SHIP_LAST)
                && (dtype < UNIT_SHIP_FIRST || dtype > UNIT_SHIP_LAST))
                special_kill(defender, u, show, x, y);
            else
                special_kill_stack(defender, u, show, x, y);
        } else if (colony >= 0) {
            f72 = (anation < NATION_EUROPEAN_COUNT);
            if ((int)g_cur_colony_rec->pop > 1 || f72 != 0) {
                if (f72 == 0)
                    delete_colonist(victim);
                if (show != 0) {
                    if (colony_taken != 0)
                        sound_play(SOUND_BIGEXPLO);
                    else
                        sound_play(SOUND_BOOM3);
                }
            } else {
                unit = top_unit_in_square(x, y);
                if (unit >= 0)
                    special_kill_stack(unit, u, show, x, y);
                if (show != 0)
                    sound_play(SOUND_BOOM3);
                if (anation >= NATION_EUROPEAN_COUNT) {
                    set_tribe(anation - 4);
                    if (g_cur_colony_rec->stock[GOODS_HORSES] != 0)
                        g_cur_tribe_rec->horse_herds++;
                    if (g_cur_colony_rec->stock[GOODS_MUSKETS] != 0)
                        g_cur_tribe_rec->muskets++;
                }
                set_name_slot_1020_a94a(3, g_cur_colony_rec->name);
                delete_colony(colony);
                destroyed = 1;
            }
        } else {
            is_capital = g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL;
            if (g_cur_settlement_rec->size > 1) {
                g_cur_settlement_rec->size--;
                if (show != 0)
                    sound_play(SOUND_LILEXPLO);
            } else {
                if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)
                    g_cur_tribe_rec->flags |= 0x40;
                delete_village(village);
                destroyed = 1;
                if (show != 0)
                    sound_play(SOUND_BOOM3);
                if ((village_mission & 0xf) == anation) {
                    unit = create_unit(UNIT_MISSIONARIES, anation, x, y);
                    if (unit >= 0 && (village_mission & 0x10))
                        g_units[unit].spec = JOB_MISSIONARY;
                }
            }
        }
        u = top_unit_in_square(-2, -2);
        u = bottom_unit(u);
        try_to_improve(u, att, def);
        if (anation >= NATION_EUROPEAN_COUNT && dnation < NATION_EUROPEAN_COUNT && made_defender == 0 && f6e == 0) {
            if ((dtype == UNIT_DRAGOONS || dtype == UNIT_SCOUTS) && (atype == UNIT_BRAVES || atype == UNIT_ARMED_BRAVES)) {
                prom2 = 1;
                g_units[u].type = g_units[u].type + 2;
                end_unit_turn(u);
                g_tribes[anation - 4].horse_herds++;
            } else if (dtype == UNIT_SOLDIERS && (atype == UNIT_MOUNTED_BRAVES || atype == UNIT_BRAVES)) {
                prom1 = 1;
                g_units[u].type = g_units[u].type + 1;
            }
        }
        move_to_bottom(u);
    }
    if (won == 0 || f6e != 0) {
        if (won == 0)
            try_to_improve(defender, def, att);
        if (won == 0)
            special_kill_stack(u, defender, show, ux, uy);
        else
            kill_stack(u);
        unit = top_unit_in_square(-2, -2);
        if (unit >= 0)
            relocate_stacked_units(u, ux, uy);
        if (won != 0 && show != 0) {
            if (a_is_ship != 0)
                sound_play(SOUND_BLTH20);
            else
                sound_play(SOUND_BLTDIRT);
        }
    } else
        relocate_stacked_units(u, ux, uy);
    if (show != 0) {
        x0 = min_int_1020_a8ad(ux, x);
        y0 = min_int_1020_a8ad(uy, y);
        w = x + 2 - x0;
        h = y + 2 - y0;
        if (f6e == 0 && won != 0)
            DrawMapRegion(ux - 1, uy - 1, 3, 3, 1);
        if (won == 0)
            DrawMapRegion(x - 1, y - 1, 3, 3, 1);
        if (destroyed != 0)
            DrawMap(0);
        DrawMapRegion(x0, y0, w, h, 0);
        ShowMapRegion(x0 - 1, y0 - 1, w + 2, h + 2);
    }
    popup_say(0, get_name(anation));
    popup_say(1, get_adjective(dnation));
    near_colony = find_close_colony(x, y, -1, -1);
    if (near_colony >= 0) {
        if (destroyed == 0)
            set_name_slot_1020_a94a(3, g_cur_colony_rec->name);
    } else
        popup_say(3, g_misc_words[MISC_WILDERNESS]);
    if (anation < NATION_EUROPEAN_COUNT && g_game.player_nation == dnation && destroyed == 0
        && colony_taken == 0) {
        if (made_defender != 0)
            popup_say(2, job_name(victim_spec));
        else
            popup_say(2, g_unit_types[dtype].name);
        if (won != 0) {
            popup_say(4, g_misc_words[MISC_DEFEAT]);
            popm("EUROPEWIN", 1);
        } else {
            if (dtype < UNIT_CONTINENTAL_CAVALRY)
                popup_say(4, g_misc_words[MISC_DEFEAT]);
            else
                popup_say(4, g_misc_words[MISC_DEFEATS]);
            force_mood(4);
            popm("EUROPELOSE", 1);
        }
    }
    euro_vs_euro = (anation < NATION_EUROPEAN_COUNT && dnation < NATION_EUROPEAN_COUNT);
    if (euro_vs_euro != 0) {
        captured = (colony_taken != 0 && destroyed == 0);
        ship_victory = (a_is_ship != 0 && d_is_ship != 0 && won != 0);
        if (destroyed != 0) {
            popup_say(0, get_name(anation));
            popup_say(1, get_name(dnation));
            if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN) {
                force_mood(1);
                woodcut(WOODCUT_COLONY_BURNING);
                popm("BURNED", 1);
            } else if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN) {
                force_mood(4);
                popm("BURNED2", 1);
            } else
                popm("BURNED3", 2);
        }
        if (captured != 0 || ship_victory != 0) {
            stack_units(u);
            move_to_bottom(u);
            unit = top_unit_in_square(x, y);
            special_kill_stack(unit, u, show, x, y);
            u = top_unit_in_square(-2, -2);
            u = bottom_unit(u);
            if (show != 0) {
                if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
                    force_mood(3);
                slide = 0xc0;
                if ((unsigned char)(g_units[u].flags & 0xf) != ((unsigned char)g_game.view_nation))
                    slide |= 0x20;
                MapWin_SlideIcon(&g_map_window, u, slide, -1, ux, uy, x, y);
            }
            relocate_stacked_units(u, x, y);
            owner_set(x, y, anation);
            if (captured != 0) {
                if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)
                    force_mood(4);
                else if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN)
                    force_mood(1);
                if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
                    if (g_game.mother_country_idx == anation)
                        g_game.game_flags |= 0x40;
                    if (g_game.player_nation == anation
                        && (g_game.hint_flags & 1) == 0) {
                        g_game.hint_flags |= 1;
                        popm("HOWTOWIN", 1);
                    }
                }
                g_nation_colony_count[g_cur_colony_rec->nation]--;
                g_pop_by_nation[g_cur_colony_rec->nation] -= g_cur_colony_rec->pop;
                g_cur_colony_rec->nation = anation;
                g_nation_colony_count[g_cur_colony_rec->nation]++;
                g_pop_by_nation[g_cur_colony_rec->nation] += g_cur_colony_rec->pop;
                g_cur_colony_rec->bells = g_cur_colony_rec->bells * 2 / 3;
                for (dir = 0; dir < 8; dir++) {
                    nx = g_dir_dx[dir] + x;
                    ny = g_dir_dy[dir] + y;
                    if (is_anything(nx, ny) < 0)
                        owner_set(nx, ny, anation);
                }
                if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && g_game.mother_country_idx == anation) {
                    for (dir = 0; dir < 8; dir++) {
                        nx = g_dir_dx[dir] + x;
                        ny = g_dir_dy[dir] + y;
                        unit = top_unit_in_square(nx, ny);
                        if (unit < 0)
                            continue;
                        if ((unsigned char)(g_units[unit].flags & 0xf)
                            != (unsigned char)anation)
                            continue;
                        while (unit > 0) {
                            g_units[unit].home = g_cur_colony_idx;
                            unit = next_unit(unit);
                        }
                    }
                }
            }
            if (show != 0)
                DrawMapRegion(x0 - 1, y0 - 1, w + 2, h + 2, 1);
        }
        if (captured != 0 && show != 0) {
            popup_say(0, get_name(anation));
            set_name_slot_1020_a94a(2, g_cur_colony_rec->name);
            popsum = g_cur_colony_rec->pop;
            for (ci = 0; g_game.colony_count > ci; ci++) {
                if ((char)g_colonies[ci].nation == (char)dnation)
                    popsum += g_colonies[ci].pop;
            }
            popsum = max_int_1020_a887(1, popsum);
            if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                gold = (long)g_cur_colony_rec->pop * g_nations[dnation].gold
                       / popsum;
                g_nations[dnation].gold -= gold;
                g_nations[anation].gold += gold;
                set_text_arg_l_1020_aa27(0, gold);
            }
            g_game.war_started[anation] = 0;
            g_game.war_started[dnation] = 0;
            if (g_nations[anation].rel[dnation] & 2)
                g_nations[anation].rel[dnation] &= ~2;
            else
                g_nations[dnation].rel[anation] |= 2;
            if ((dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN)
                || (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)) {
                if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
                    popm("CAPTURED3", 1);
                else
                    popm("CAPTURED", 1);
            } else
                popm("CAPTURED2", 2);
            if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)
                colony_screen(colony);
        }
    }
    if (anation >= NATION_EUROPEAN_COUNT && dnation < NATION_EUROPEAN_COUNT) {
        set_tribe(anation - 4);
        if (near_colony >= 0 && g_nearest_dist == 0 && won == 0)
            resolve_raid(anation, near_colony, a_f6, f82, atype);
        else {
            delta = 0;
            g_settlements[a_f6].anger[dnation] = 0;
            if (made_defender != 0) {
                popup_say(2, job_name(victim_spec));
                if (destroyed != 0) {
                    if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN) {
                        sound_play(SOUND_FIRE1);
                        force_music(0x32);
                        woodcut(WOODCUT_COLONY_BURNING);
                    }
                    if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN)
                        popm("INDIANBURNCOLONY", 1);
                    else
                        popm("INDIANBURNCOLONY2", 3);
                    delta = -0x32;
                } else {
                    if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN) {
                        force_music(0x32);
                        popm("INDIANWINCOLONY", 1);
                    } else
                        popm("INDIANWINCOLONY2", 3);
                    delta = (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN
                             ? g_game.difficulty : 0) - 10;
                }
            } else {
                if (made_defender != 0)
                    popup_say(2,
                        job_name(victim_spec));
                else
                    popup_say(2, g_unit_types[dtype].name);
                if (won != 0) {
                    STRCPY(buf, "INDIANWIN0");
                    if (prom1 != 0)
                        buf[9] = '1';
                    if (prom2 != 0)
                        buf[9] = '2';
                    if (prom1 != 0 || prom2 != 0)
                        popup_say(4,
                            get_adjective(anation));
                    if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN) {
                        force_music(0x32);
                        popm(buf, 1);
                    }
                    delta = (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN
                             ? g_game.difficulty >> 1 : 0) - 5;
                } else {
                    if (dtype < UNIT_CONTINENTAL_CAVALRY)
                        popup_say(4, g_misc_words[MISC_DEFEAT]);
                    else
                        popup_say(4, g_misc_words[MISC_DEFEATS]);
                    if (dnation < NATION_EUROPEAN_COUNT && g_nation_text[dnation].control == CONTROL_HUMAN) {
                        force_mood(4);
                        popm("INDIANLOSE", 1);
                    }
                }
            }
            if (delta != 0 && !(get_treaty(g_cur_tribe_nation, dnation) & 2))
                piss_off(g_cur_tribe, dnation, delta, 0);
        }
    }
    if (anation < NATION_EUROPEAN_COUNT && dnation >= NATION_EUROPEAN_COUNT && won != 0) {
        if (made_defender != 0 && village_mission >= 0 && (village_mission & 0xf) == anation) {
            chance = 4;
            if (village_mission & 0x10)
                chance += 4;
            if (anation == NATION_SPAIN)
                chance += 4;
            if (has_father(anation, FATHER_JUAN_DE_SEPULVEDA))
                chance += 4;
            if (has_father(anation, FATHER_BARTOLOME_DE_LAS_CASAS))
                chance -= 4;
            f70 = (rand_range(0, 0xc) < chance);
            if (f70 != 0) {
                if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN) {
                    popup_say(0, get_name(dnation));
                    popup_say(1, get_adjective(anation));
                    popm("INDIANSLAVES", 1);
                }
                unit = create_unit(UNIT_COLONISTS, anation, ux, uy);
                if (unit >= 0) {
                    g_units[unit].spec = JOB_CONVERT;
                    if (show != 0 && destroyed == 0) {
                        DrawMapRegion(x0, y0, w, h, 0);
                        ShowMapRegion(x0 - 1, y0 - 1, w + 2, h + 2);
                    }
                }
            }
        }
        if (destroyed != 0) {
            if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN)
                force_mood(4);
            g_nations[anation].villages_burned++;
            set_tribe(dnation - 4);
            flagA = has_father(anation, FATHER_HERNAN_CORTES);
            f7a = (anation == NATION_SPAIN);
            gift = 0;
            switch (g_cur_tribe_rec->level) {
            case 0:
                chance = f7a != 0 ? 3 : 6;
                if (rand_range(0, chance) == 0 || is_capital != 0 || flagA != 0)
                    gift = rand_range(2, 4);
                if (is_capital != 0)
                    gift = gift * 2;
                if (flagA != 0)
                    gift += gift >> 1;
                break;
            case 1:
                chance = f7a != 0 ? 1 : 2;
                if (rand_range(0, 2) == 0 || is_capital != 0 || flagA != 0)
                    gift = rand_range(3, 8);
                if (is_capital != 0)
                    gift = gift * 2;
                if (flagA != 0)
                    gift += gift >> 1;
                break;
            case 2:
                gift = ((is_capital != 0 ? rand_range(4, 0xa) : rand_range(2, 6))
                        + (flagA != 0 ? 6 : 0) + (f7a != 0 ? 3 : 0)) * 10;
                break;
            case 3:
                gift = rand_range(0, 4) + 2;
                gift = ((is_capital != 0 ? 0x19 : 0x10) + (flagA != 0 ? 0xa : 0)
                        + (f7a != 0 ? 5 : 0)) * gift;
                break;
            }
            kind = is_capital != 0 ? 4 : g_cur_tribe_rec->level;
            popup_say(0, get_adjective(anation));
            popup_say(1, get_adjective(dnation));
            popup_say(2, g_level[kind].settlement);
            if (gift != 0) {
                unit = create_unit(UNIT_TREASURE_TRAIN, anation, x, y);
                if (unit >= 0) {
                    g_units[unit].spec = gift;
                    set_text_arg_i_1020_a926(0, gift * 100);
                    force_mood(2);
                    popm("LOOT", 1);
                    free_galleon(anation);
                } else
                    gift = 0;
            } else
                popm("LOOT2", 1);
            if (show != 0) {
                DrawMapRegion(x0, y0, w, h, 0);
                if (gift != 0 || f70 != 0)
                    ShowMapRegion(x0 - 1, y0 - 1, w + 2, h + 2);
            }
        }
        if (destroyed != 0 && is_capital != 0) {
            delta = 0xf - pissed(g_cur_tribe, anation);
            if (delta < 0)
                piss_off(g_cur_tribe, anation, delta, 0);
            for (ci = 0; g_game.settlement_count > ci; ci++) {
                if ((char)g_settlements[ci].owner == (char)g_cur_tribe)
                    g_settlements[ci].anger[anation] = 0;
            }
            if (anation < NATION_EUROPEAN_COUNT && g_nation_text[anation].control == CONTROL_HUMAN) {
                popup_say(0, get_adjective(dnation));
                popup_say(1, get_adjective(anation));
                popi("INDIANBOW", g_cur_tribe);
            }
        }
    }
    goto out;
vanish:
    if (on_map(x, y) == 0)
        goto out;
    unit = top_unit_in_square(x, y);
    if (unit >= 0)
        kill_stack(unit);
    unit = find_colony(x, y);
    if (unit >= 0)
        delete_colony(unit);
    unit = find_village(x, y);
    if (unit >= 0)
        delete_village(unit);
    feature_set(x, y, MAP1_UNIT, 0);
    feature_set(x, y, MAP1_SETTLEMENT, 0);
    if (show != 0) {
        DrawMap(0);
        ShowMap();
    }
out:
    ;
}
