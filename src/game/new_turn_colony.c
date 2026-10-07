/* new_turn_colony: candidate for 1018:9476 (6636 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: new_turn_colony
 * name-source: sibling-build
 *
 * EVERYTHING ONE COLONY DOES BETWEEN TURNS, in one function: it produces and consumes, sells
 * through the custom house, rings its bells, teaches and promotes its people, grows or starves,
 * builds, spoils what will not fit in the warehouse, and wears out its mine. It is the largest
 * function in segment 1018 and the single place the colony simulation lives.
 *
 * THE GOODS LOOP runs the sixteen commodities. `net_cargo_change` gives the net for the turn,
 * except for a colony that is not a live European power -- nation 4 and up, or one whose
 * `NationText.control` is non-zero -- where the net is taken from the tables at SEG21:0x122 and
 * SEG21:0x164 instead, and food gets half the difficulty level thrown in on top. The new total is
 * floored at zero and each good that moved sets its bit in the colony's +0x90 word, which is what
 * the colony window reads to know which numbers to flash.
 *
 * THE CUSTOM HOUSE is the block guarded by colony flag 0x12. A stock of 100 or more is cut back to
 * 50 and the difference sold at the European price; the tax is `gross * (long)Nation.tax / 100`,
 * the tax goes to the King's chest at +0x22 and the overflow sale credits your gold at +0x2a, and
 * the rest reaches the treasury through `add_gold`. Only a live European colony
 * gets the sentence written into the map window -- amount, goods, gross, and, when the tax is not
 * suppressed by SEG20:0x7784 bit 0, the rate, the tax and the net.
 *
 * BELLS AND SONS OF LIBERTY. Both counters at +0xc2 and +0xc6 lose a sixty-fourth of themselves
 * every turn before the new bells go in, which is the decay that makes a colony slide back when it
 * stops producing. The membership is then compared against its own previous value through a
 * ten-armed if-else chain: crossing 50 and crossing 100 each announce once and latch a bit in
 * +0x1c, dropping below 95 and below 50 clear them again, and if none of those fired a change in
 * the TENS digit announces SONSUP or SONSDOWN. Every arm is doubled -- one with the message and one
 * without -- because SEG20:0x7786 bit 9 decides whether this nation is told at all, and the bit is
 * read once into `rebel` at the top.
 *
 * TORIES are `pop * (100 - membership) / 100` and the tolerated number is `10 - difficulty`, so the
 * hardest level (4, Viceroy) starts complaining at six malcontents and the easiest (0, Discoverer) at ten.
 *
 * TEACHING is two passes. The first walks the colonists, bumps each one's experience nibble, lists
 * every criminal, servant and free colonist (JOB_COLONIST or JOB_COLONIST_ALT) in `teach[]`, and lists up to three
 * schoolhouse teachers in `spec_list[]` -- a teacher qualifies once the nibble reaches 4, 6 or 8
 * depending on the profession's rank word. The second pass hands each taught profession to a
 * RANDOMLY CHOSEN pupil, promoting criminal to servant and servant to free colonist rather than
 * straight to the profession, and closes the gap in `teach[]` by shifting the tail down.
 *
 * ON-THE-JOB PROMOTION is separate and comes after: a colonist doing one of the four farm jobs, in
 * a colony where nobody has yet been promoted into that job this turn, has a 1-in-100 chance, 1-in-200
 * as a servant and 1-in-300 as a criminal.
 *
 * STARVATION. The food shortfall at SEG21:0x1b4 is ignored for an AI or native colony when it is
 * under three. Otherwise the message depends on the season (`g_game.season == 0` picks FOOD1/STARVE1, otherwise the winter-is-coming FOOD2/STARVE2) and on how many
 * colonists die -- and `starve` is 1, not the real shortfall, so a colony never loses more than one
 * settler a turn and VANISH is only reached by a colony of one. The counter is also cleared when
 * there was food in store at the START of the turn, and on the two easiest levels a colony before
 * 1520 never starves at all.
 *
 * CONSTRUCTION needs both hammers and tools. Having them, an item the colony already owns sets
 * +0x1c bit 7 and says ALREADYHAVE; not having the tools says NEEDTOOLS -- with a "0" glued on the
 * end when there are none at all, so the resource is NEEDTOOLS or NEEDTOOLS0 -- and an AI colony
 * simply gets the tools handed to it.
 *
 * SPOILAGE is the overflow above the warehouse cap. An AI or native colony sells its overflow into
 * the world market instead: muskets become fifty-per-soldier on the nation record and horses go
 * straight to the nation's herd. What is left over the cap decays by the turn's production plus
 * whatever else fits, and a single spoiling good is named in SPOIL1 while two or more get SPOIL2.
 *
 * CARGOREADY is the player's "a full hold is waiting" message, raised whenever a good crosses a
 * hundred mark; the resource is CARGOREADY0, 1 or 2 with the last digit overwritten in the buffer
 * according to whether the stock is exactly at the cap and whether the cap is 300.
 *
 * TWO SHIPPED BUGS worth naming. The overflow accounting does `Nation.trade_tons[i] += i` -- it adds the
 * GOODS INDEX to the nation's per-goods long rather than the amount sold, so that column counts
 * nothing meaningful. And `starve` is assigned 0 and then 1 on the next line, so the first
 * assignment is dead; the shipped code writes both.
 *
 * WHAT THE BYTES SETTLED, four spellings that are invisible in C:
 *   `!(x & k)` and `(x & k)` are DIFFERENT for a `char` field. The truth test compiles to
 *   `TEST BYTE PTR`, while `!` promotes first -- `MOV AL / MOV AH,0 / TEST AL,k` -- and the
 *   shipped code uses each form consistently: every "flag is clear" test in the SoL chain is a
 *   `!`, every "flag is set" test is the plain form. `(x & k) == 0` gives the byte test, not the
 *   promoted one, so it is not the same source.
 *   The tax cast is on the RATE, not the amount: `gross * (long)Nation.tax` sign-extends the byte
 *   straight to 32 bits (`MOVSX EDX`), where `(long)gross * Nation.tax` goes through CBW/CWDE and
 *   spills.
 *   `useful` is a CONDITIONAL EXPRESSION, not an if-else: the shipped stores AX once after the
 *   join, where two assignments store twice.
 *   And the nation's CROSS total is `n.crosses = n.crosses + g_prod[PROD_CROSSES]`, not `+=`: the plain form
 *   loads the destination first and the compound form loads the addend first (rule 60).
 *
 * RENAMED 2026-09-26 from colony_end_of_turn: the Mac build's new_turn_colony(short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (callee AddNumberToMessage; callee AddToMessage; callee BackspaceMessage; callee CancelMessage; callee Show), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "difficul.h"
#include "goods.h"
#include "nations.h"
#include "control.h"
#include "tutflag.h"
#include "colflag.h"
#include "colopt.h"
#include "units.h"
#include "building.h"
#include "gameflag.h"
#include "jobs.h"
#include "aicol.h"
#include "misc.h"
#include "labels.h"
#include "w16types.h"
#include "records.h"

typedef struct { char colchild[0x77a]; short cx; short cy; char pad_77e[4]; short zoom; short drag; short lbutton_held; } MapWin;   /* MapWin : ColChild, the base as its bytes: 0x788 */

typedef struct { char pad[1]; } ColonyWin;

extern GameState g_game;   /* SEG20:0x7782 */
extern Nation g_nations[];                /* SEG20:0xc2f4, 0x13c bytes each */

extern int g_consumption[];                     /* SEG21:0x164 */
extern int g_deficit[];                   /* SEG21:0x18c */
extern int g_shortfall[];                 /* SEG21:0x1b4 */

extern void set_europe(int n);

extern int europe_bid_price(int good);
extern void traffic_to_europe(int good, int amount);
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; } ColChild;   /* ColChild : GameWin, the base and the SImageButton as their bytes: 0x77a */
extern void ColChild_CancelMessage(ColChild *child, int a);
extern void ColChild_AddStringToMessage(ColChild *message, char *s);
extern void ColChild_AddToMessage(ColChild *child, int word);

extern void ColChild_BackspaceMessage(ColChild *w);

extern void popup_say(int slot, int word);
extern void set_mood(int mood);

extern void random_activate(int seed);
extern void new_mood(int mood);
extern void force_mood(int mood);

extern char *strings(int n);
extern int popm(char *msg, int n);
extern void ColonyWin_Show(void *w, int colony);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_colony(int colony);
extern "C" void colony_fortress(void);
extern "C" void colony_indian_table(void);
extern "C" void compute_colony_parameters(void);
extern "C" int net_cargo_change(int idx, long out);
extern "C" int get_warehouse(void);

extern "C" int has_building(int bit);
extern "C" int custom_sell(int n);
extern "C" int ai_sell(int goods);
extern "C" void add_gold(int nation, long amount);
extern "C" int colony_rebel(void);

extern "C" char possible_popup(char *msg, int a, int b, int c, int d, int e, int f);
extern "C" int get_progress(int u);
extern "C" int get_job(int u);
extern "C" int get_specialty(int u);
extern "C" void set_progress(int u, int progress);

extern "C" void set_specialty(int u, int spec);
extern "C" int any_spec(int job);
extern "C" int food_box_size(int *pa, int *pb);
extern "C" void delete_colonist(int u);
extern "C" void delete_colony(int c);
extern "C" int get_production_cost(int item, int *out);
extern "C" int get_production_class(int item, int *idx);
extern "C" char *get_production_name(int item);
extern "C" void produce_something(void);
extern "C" int job_count(int job);
extern "C" void colony_depletion(void);

void new_turn_colony(int colony)
{
    register int u;
    int i;
    int grow;
    int unit;
    int job;
    int spec2;
    int kind;
    int need;
    int nib;
    int food0;
    int starve;
    int victim;
    int first;
    int cap;
    int over;
    int excess;
    int spoil;
    int spoil_good;
    int nspoil;
    int spec;
    int delta;
    int pick;
    int chance;
    int tools;
    int budget;
    int nation;
    int amount;
    int useful;
    int kind2;
    int item;
    int bells;
    int gross;
    int tax;
    int net;
    int sol;
    int rebel;
    int tories;
    int limit;
    int sol_before;
    int spec_list[3];
    int nteach = 0;
    int nspec = 0;
    int start[16];
    int prod[16];
    int teach[32];
    char buf[0x50];

    g_msg_seen = 0;
    set_colony(colony);
    nation = g_cur_colony_rec->nation;
    set_europe(nation);
    colony_fortress();
    colony_indian_table();
    compute_colony_parameters();
    bells = net_cargo_change(0x12, 0L);
    add_liberty(nation, bells);
    cap = get_warehouse();
    food0 = g_cur_colony_rec->stock[GOODS_FOOD];
    g_cur_colony_rec->changed = 0;
    for (i = 0; i < GOODS_COUNT; i++) {
        start[i] = g_cur_colony_rec->stock[i];
        delta = net_cargo_change(i, 0L);
        if (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN)
            delta = g_prod[i] - g_consumption[i];
        if ((nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN) && i == 0)
            delta += g_game.difficulty >> 1;
        g_cur_colony_rec->stock[i] += delta;
        g_cur_colony_rec->stock[i] = max_int_1018_b34f(0, g_cur_colony_rec->stock[i]);
        if (has_building(BUILDING_CUSTOM_HOUSE)) {
            if (!(g_cur_colony_rec->ai_flags & (COLONY_AI_BESIEGED | COLONY_AI_BLOCKADED)) || nation >= NATION_EUROPEAN_COUNT
                || g_nation_text[nation].control != CONTROL_HUMAN) {
                useful = (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
                         ? custom_sell(i)
                         : ai_sell(i);
                if (useful != 0 && g_cur_colony_rec->stock[i] >= 0x64) {
                    amount = g_cur_colony_rec->stock[i] - 0x32;
                    g_cur_colony_rec->stock[i] -= amount;
                    delta -= amount;
                    gross = europe_bid_price(i) * amount;
                    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                        tax = gross * (long)g_cur_nation_rec->tax / 100;
                        net = gross - tax;
                    } else {
                        tax = 0;
                        net = gross;
                    }
                    add_gold(g_cur_nation, (long)net);
                    traffic_to_europe(i, amount);
                    g_cur_nation_rec->royal_gold += tax;
                    g_cur_nation_rec->net_take += net;
                    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                        ColChild_CancelMessage((ColChild *)&g_map_window, 1);
                        ColChild_AddStringToMessage((ColChild *)&g_map_window, g_cur_colony_rec->name);
                        ColChild_AddToMessage((ColChild *)&g_map_window, g_misc_words[MISC_SELLS]);
                        ColChild_AddNumberToMessage((ColChild *)&g_map_window, amount);
                        ColChild_AddToMessage((ColChild *)&g_map_window, g_goods_name_word[i]);
                        ColChild_AddToMessage((ColChild *)&g_map_window, g_misc_words[MISC_FOR]);
                        ColChild_AddNumberToMessage((ColChild *)&g_map_window, gross);
                        ColChild_BackspaceMessage((ColChild *)&g_map_window);
                        ColChild_AddStringToMessage((ColChild *)&g_map_window, ".");
                        if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                            ColChild_AddNumberToMessage((ColChild *)&g_map_window, g_cur_nation_rec->tax);
                            ColChild_BackspaceMessage((ColChild *)&g_map_window);
                            ColChild_AddToMessage((ColChild *)&g_map_window, g_colony_message_words[CMSG_TAX]);
                            ColChild_AddNumberToMessage((ColChild *)&g_map_window, tax);
                            ColChild_BackspaceMessage((ColChild *)&g_map_window);
                            ColChild_AddToMessage((ColChild *)&g_map_window, g_colony_message_words[CMSG_NET]);
                            ColChild_AddNumberToMessage((ColChild *)&g_map_window, net);
                        }
                        viceroy_activate_message(1, 0xaL);
                    }
                }
            }
        }
        if (i != 0) {
            prod[i] = g_cur_colony_rec->stock[i] - cap;
            prod[i] = minimax(prod[i], 0, delta);
        } else
            prod[i] = 0;
        if (g_prod[i] != 0 && (delta > 0 || (delta == 0 && prod[i] != 0)))
            g_cur_colony_rec->changed |= 1 << i;
    }
    sol_before = colony_rebel();
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0 && g_cur_colony_rec->nation == ((unsigned char)g_game.mother_country_idx))
        bells = -(bells >> 1);
    else if (g_cur_colony_rec->pop > bells)
        bells -= sol_before / 0x14;
    g_cur_colony_rec->bells -= g_cur_colony_rec->bells >> 6;
    g_cur_colony_rec->bells_cap -= g_cur_colony_rec->bells_cap >> 6;
    g_cur_colony_rec->bells_cap = max_long_1018_b375(1L, g_cur_colony_rec->bells_cap);
    g_cur_colony_rec->bells += bells;
    g_cur_colony_rec->bells_cap += g_cur_colony_rec->pop * 2;
    g_cur_colony_rec->bells = max_long_1018_b375(g_cur_colony_rec->bells, 0L);
    g_cur_colony_rec->bells = min_long_1018_b3a3(g_cur_colony_rec->bells,
                                                 g_cur_colony_rec->bells_cap);
    sol = colony_rebel();
    set_text_arg_i_1018_b472(0, sol);
    rebel = ((g_game.colony_options & COLOPT_NO_REBEL_MAJORITY) == 0x200);
    if (sol >= 0x32 && !(g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel != 0) {
        popup_say(1, g_nation_name[nation]);
        g_msg_seen |= possible_popup("REBELMAJORITY", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status |= COLONY_REBEL_MAJORITY;
    } else if (sol >= 0x32 && !(g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel == 0) {
        g_cur_colony_rec->status |= COLONY_REBEL_MAJORITY;
    } else if (sol >= 0x64 && !(g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel != 0) {
        popup_say(1, g_nation_name[nation]);
        g_msg_seen |= possible_popup("REBELUNANIMOUS", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status |= COLONY_REBEL_UNANIMOUS;
    } else if (sol >= 0x64 && !(g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel == 0) {
        g_cur_colony_rec->status |= COLONY_REBEL_UNANIMOUS;
    } else if (sol < 0x5f && (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel != 0) {
        popup_say(1, g_nation_name[nation]);
        g_msg_seen |= possible_popup("TORYMINORITY", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status &= ~COLONY_REBEL_UNANIMOUS;
    } else if (sol < 0x5f && (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel == 0) {
        g_cur_colony_rec->status &= ~COLONY_REBEL_UNANIMOUS;
    } else if (sol < 0x32 && (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel != 0) {
        popup_say(1, g_nation_name[nation]);
        g_msg_seen |= possible_popup("TORYMAJORITY", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status &= ~COLONY_REBEL_MAJORITY;
    } else if (sol < 0x32 && (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel == 0) {
        g_cur_colony_rec->status &= ~COLONY_REBEL_MAJORITY;
    } else if (sol / 10 > sol_before / 10) {
        if ((g_game.colony_options & COLOPT_NO_SONS_OF_LIBERTY) != 0x100)
            g_msg_seen |= possible_popup("SONSUP", !g_msg_seen, -1, 0, 0, 1, 0);
    } else if ((sol + 4) / 10 < sol_before / 10) {
        if ((g_game.colony_options & COLOPT_NO_SONS_OF_LIBERTY) != 0x100)
            g_msg_seen |= possible_popup("SONSDOWN", !g_msg_seen, -1, 0, 0, 1, 0);
    }
    tories = g_cur_colony_rec->pop * (100 - sol) / 100;
    limit = 10 - g_game.difficulty;
    if (tories >= limit) {
        if (!(g_cur_colony_rec->status & COLONY_INEFFICIENT) && (g_game.colony_options & COLOPT_NO_BAD_GOVERNMENT) == 0) {
            set_text_arg_i_1018_b472(0, limit);
            g_msg_seen |= possible_popup("INEFFICIENT", !g_msg_seen, -1, 0, 0, 5, 0);
        }
        g_cur_colony_rec->status |= COLONY_INEFFICIENT;
    } else {
        if ((g_cur_colony_rec->status & COLONY_INEFFICIENT) && (g_game.colony_options & COLOPT_NO_BAD_GOVERNMENT) == 0)
            g_msg_seen |= possible_popup("EFFICIENT", !g_msg_seen, -1, 0, 0, 5, 0);
        g_cur_colony_rec->status &= ~COLONY_INEFFICIENT;
    }
    for (i = 0; i < GOODS_COUNT; i++) {
        if (i != 0) {
            prod[i] = min_int_1018_b329(prod[i], g_cur_colony_rec->stock[i] - cap);
            prod[i] = max_int_1018_b34f(0, prod[i]);
        }
    }
    for (i = 0; g_cur_colony_rec->pop > i; i++) {
        nib = get_progress(i);
        job = get_job(i);
        spec = get_specialty(i);
        nib++;
        if (spec == JOB_CRIMINAL || spec == JOB_INDENTURED_SERVANT || spec == JOB_COLONIST_ALT || spec == JOB_COLONIST)
            teach[nteach++] = i;
        if (job == JOB_TEACHER && nspec < 3) {
            if (spec == JOB_COLONIST_ALT)
                spec = JOB_INDENTURED_SERVANT;
            kind = g_spec_recs[spec].school;
            if (kind < SCHOOL_NONE) {
                switch (kind) {
                case SCHOOL_SCHOOLHOUSE:
                    need = 4;
                    break;
                case SCHOOL_COLLEGE:
                    need = 6;
                    break;
                case SCHOOL_UNIVERSITY:
                default:
                    need = 8;
                    break;
                }
                if (nib >= need) {
                    spec_list[nspec++] = spec;
                    nib = 0;
                }
            }
        }
        set_progress(i, nib);
    }
    for (i = 0; i < nspec; i++) {
        if (nteach == 0) {
            g_msg_seen |= possible_popup("TRAINFAIL", !g_msg_seen, -1, 0, 0, 2, 0x8025);
            break;
        }
        if (g_cur_colony_is_players != 0)
            set_mood(2);
        pick = rand_range(0, nteach - 1);
        u = teach[pick];
        set_name_slot_1018_b3f6(0, g_cur_colony_rec->name);
        spec = get_specialty(u);
        if (spec == JOB_CRIMINAL) {
            set_specialty(u, JOB_INDENTURED_SERVANT);
            if ((g_game.colony_options & COLOPT_NO_TRAINED) == 0)
                g_msg_seen |= possible_popup("TRAINCRIMINAL", !g_msg_seen, -1, 0, 0, 2, 0x8025);
        } else if (spec == JOB_INDENTURED_SERVANT) {
            set_specialty(u, JOB_COLONIST_ALT);
            if ((g_game.colony_options & COLOPT_NO_TRAINED) == 0)
                g_msg_seen |= possible_popup("TRAININDENTURED", !g_msg_seen, -1, 0, 0, 2, 0x8025);
        } else {
            set_specialty(u, spec_list[i]);
            popup_say(1, g_spec_recs[spec_list[i]].name);
            if ((g_game.colony_options & COLOPT_NO_TRAINED) == 0)
                g_msg_seen |= possible_popup("TRAINPROFESSION", !g_msg_seen, -1, 0, 0, 2, 0x8025);
        }
        for (u = pick; nteach - 1 > u; u++)
            teach[u] = teach[u + 1];
        nteach--;
    }
    for (i = 0; g_cur_colony_rec->pop > i; i++) {
        job = get_job(i);
        spec2 = get_specialty(i);
        if (spec2 == JOB_CONVERT)
            continue;
        if (any_spec(spec2))
            continue;
        if (job < 1 || job > 4)
            continue;
        if (g_specialty_tally[job] != 0)
            continue;
        chance = 0x63;
        if (spec2 == JOB_INDENTURED_SERVANT)
            chance += 0x64;
        if (spec2 == JOB_CRIMINAL)
            chance += 0xc8;
        if (rand_range(0, chance) != 0)
            continue;
        g_specialty_tally[job]++;
        set_specialty(i, job);
        popup_say(1, g_spec_recs[job].name);
        if ((g_game.colony_options & COLOPT_NO_TRAINED) == 0)
            g_msg_seen |= possible_popup("TRAINPROFESSION", !g_msg_seen, -1, 0, 0, 2, 0x8025);
    }
    grow = food_box_size(0, 0);
    if (g_cur_colony_rec->stock[GOODS_FOOD] >= grow) {
        g_cur_colony_rec->stock[GOODS_FOOD] -= grow;
        unit = create_unit(UNIT_COLONISTS, nation, g_cur_colony_rec->x, g_cur_colony_rec->y);
        g_msg_seen |= possible_popup("NEWCOLONIST", !g_msg_seen, -1, 0, 0, 4, 0);
    }
    if ((nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN) && g_shortfall[GOODS_FOOD] < 3)
        g_shortfall[GOODS_FOOD] = 0;
    if (g_shortfall[GOODS_FOOD] != 0) {
        random_activate(g_random_seed);
        starve = 0;
        first = (g_game.season == 0);
        starve = 1;
        if (food0 != 0)
            starve = 0;
        if (g_game.difficulty <= DIFFICULTY_EXPLORER) {
            if (g_game.year < 0x5f0 || rand_range(0, 2 - g_game.difficulty) != 0)
                starve = 0;
        }
        if (starve == 0) {
            if (g_cur_colony_is_players != 0)
                new_mood(1);
            if (first != 0)
                g_msg_seen |= possible_popup("FOOD1", !g_msg_seen, 0, 0, 0, 5, 0);
            else
                g_msg_seen |= possible_popup("FOOD2", !g_msg_seen, 0, 0, 0, 5, 0);
        } else if (g_cur_colony_rec->pop == starve) {
            if (g_cur_colony_is_players != 0)
                force_mood(1);
            g_msg_seen |= possible_popup("VANISH", 0, -1, 0, 0, 5, 0);
        } else {
            if (g_cur_colony_is_players != 0)
                force_mood(1);
            if (first != 0)
                g_msg_seen |= possible_popup("STARVE1", !g_msg_seen, 0, 0, 0, 5, 0);
            else
                g_msg_seen |= possible_popup("STARVE2", !g_msg_seen, 0, 0, 0, 5, 0);
        }
        for (i = 0; i < starve; i++) {
            victim = rand_range(0, g_cur_colony_rec->pop - 1);
            delete_colonist(victim);
        }
        if (g_cur_colony_rec->pop == 0) {
            delete_colony(g_cur_colony_idx);
            goto out;
        }
    } else if (g_deficit[GOODS_FOOD] != 0 && g_cur_colony_rec->stock[GOODS_FOOD] < g_deficit[GOODS_FOOD] * 4
               && (g_game.colony_options & COLOPT_NO_FOOD_SHORTAGE) == 0) {
        if (g_cur_colony_is_players != 0)
            set_mood(1);
        set_text_arg_i_1018_b472(0, g_cur_colony_rec->stock[GOODS_FOOD]);
        g_msg_seen |= possible_popup("FOODLOW", !g_msg_seen, -1, 0, 0, 5, 0);
    }
    if ((g_game.colony_options & COLOPT_NO_MATERIALS_SHORTAGE) == 0) {
        if (g_shortfall[GOODS_LUMBER] != 0 && net_cargo_change(0x10, 0L) == 0)
            g_msg_seen |= possible_popup("LUMBER", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_shortfall[GOODS_COTTON] != 0 && net_cargo_change(0xb, 0L) == 0)
            g_msg_seen |= possible_popup("COTTON", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_shortfall[GOODS_TOBACCO] != 0 && net_cargo_change(0xa, 0L) == 0)
            g_msg_seen |= possible_popup("TOBACCO", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_shortfall[GOODS_SUGAR] != 0 && net_cargo_change(9, 0L) == 0)
            g_msg_seen |= possible_popup("CANESUGAR", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_shortfall[GOODS_FURS] != 0 && net_cargo_change(0xc, 0L) == 0)
            g_msg_seen |= possible_popup("FURS", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_shortfall[GOODS_ORE] != 0
            && net_cargo_change(0xe, 0L) - net_cargo_change(0xf, 0L) == 0)
            g_msg_seen |= possible_popup("ORE", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_shortfall[GOODS_TOOLS] != 0 && net_cargo_change(0xf, 0L) == 0)
            g_msg_seen |= possible_popup("TOOLS", !g_msg_seen, 0, 0, 0, 5, 0);
    }
    g_cur_colony_rec->hammers += net_cargo_change(0x10, 0L);
    g_cur_colony_rec->hammers = max_int_1018_b34f(0, g_cur_colony_rec->hammers);
    if (get_production_cost(g_cur_colony_rec->item, &tools) <= g_cur_colony_rec->hammers
        && (int)g_cur_colony_rec->item >= 0) {
        kind2 = get_production_class(g_cur_colony_rec->item, &item);
        if (kind2 == 1 && has_building(item)) {
            g_cur_colony_rec->status |= COLONY_BUILD_DONE;
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                set_name_slot_1018_b3f6(1, get_production_name(g_cur_colony_rec->item));
                g_msg_seen |= possible_popup("ALREADYHAVE", !g_msg_seen, 2, 0, 0, 5, 0);
            }
            goto after_cap;
        }
        if (g_cur_colony_rec->stock[GOODS_TOOLS] < tools) {
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                if ((g_game.colony_options & COLOPT_NO_TOOLS_NEEDED) == 0) {
                    set_name_slot_1018_b3f6(1, get_production_name(g_cur_colony_rec->item));
                    set_text_arg_i_1018_b472(0, tools);
                    set_text_arg_i_1018_b472(1, g_cur_colony_rec->stock[GOODS_TOOLS]);
                    STRCPY(buf, "NEEDTOOLS");
                    if (g_cur_colony_rec->stock[GOODS_TOOLS] == 0)
                        STRCAT(buf, "0");
                    g_msg_seen |= possible_popup(buf, !g_msg_seen, 2, 0, 0, 5, 0);
                }
            } else
                g_cur_colony_rec->stock[GOODS_TOOLS] = tools;
        }
        if (g_cur_colony_rec->stock[GOODS_TOOLS] >= tools) {
            g_cur_colony_rec->stock[GOODS_TOOLS] -= tools;
            produce_something();
        }
    }
    cap = get_warehouse();
after_cap:
    g_nations[nation].crosses = g_nations[nation].crosses + g_prod[PROD_CROSSES];
    budget = (job_count(5) * 2 + ((int)g_center_kind == 5)) * 5 >> 1;
    if (g_cur_colony_rec->stock[GOODS_LUMBER] > cap) {
        over = g_cur_colony_rec->stock[GOODS_LUMBER] - cap;
        if (over <= prod[5])
            budget -= over;
        else
            budget -= prod[5];
        budget = max_int_1018_b34f(0, budget);
    }
    nspoil = 0;
    for (i = 0; i < GOODS_COUNT; i++) {
        if (i == 0)
            continue;
        over = g_cur_colony_rec->stock[i] - cap;
        if (over > 0) {
            if (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_colony_rec->nation].control != CONTROL_HUMAN) {
                excess = over;
                if (i == 0xf) {
                    while (excess >= 0x32) {
                        excess -= 0x32;
                        g_nations[g_cur_colony_rec->nation].muskets++;
                    }
                }
                if (i == 8) {
                    g_nations[g_cur_colony_rec->nation].horses += excess;
                    excess = 0;
                }
                traffic_to_europe(i, excess);
                gross = g_goods_value[g_cur_colony_rec->nation][i] * excess;
                g_cur_nation_rec->trade_tons[i] += i;
                g_cur_nation_rec->trade_gold[i] += gross;
                g_cur_nation_rec->gold += gross;
            }
            if (prod[i] >= over) {
                g_cur_colony_rec->stock[i] = cap;
                continue;
            }
            g_cur_colony_rec->stock[i] -= prod[i];
            over -= prod[i];
            spoil = over;
            spoil = min_int_1018_b329(spoil, g_cur_colony_rec->stock[i]);
            if (spoil < 2)
                spoil = 0;
            if (spoil != 0) {
                nspoil++;
                spoil_good = i;
            }
            g_cur_colony_rec->stock[i] -= spoil;
        }
    }
    if (nspoil != 0) {
        if (nspoil == 1) {
            g_text_args[0] = spoil;
            STRCPY(g_name_slots[1], strings(g_goods_name_word[spoil_good]));
            g_msg_seen |= possible_popup("SPOIL1", !g_msg_seen, 0, 0, 0, 5, 0);
        } else
            g_msg_seen |= possible_popup("SPOIL2", !g_msg_seen, 0, 0, 0, 5, 0);
    }
    if (g_cur_colony_is_players != 0 && (g_game.colony_options & COLOPT_NO_NEW_CARGO) == 0) {
        for (i = 0; i < GOODS_COUNT; i++) {
            if (start[i] / 100 < g_cur_colony_rec->stock[i] / 100 && i != 0) {
                set_mood(2);
                popup_say(1, g_goods_name_word[i]);
                g_text_args[0] = cap;
                STRCPY(buf, "CARGOREADY0");
                if (g_cur_colony_rec->stock[i] == cap)
                    buf[10] = '1';
                if (g_cur_colony_rec->stock[i] == cap && cap == 0x12c)
                    buf[10] = '2';
                g_msg_seen |= possible_popup(buf, !g_msg_seen, -1, 0, 0, 5, 0);
                if (g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT
                    && g_nation_text[g_cur_colony_rec->nation].control == CONTROL_HUMAN
                    && (g_game.game_flags & GAMEFLAG_TUTORIAL) && (g_game.hint_flags & TUT_SHOWN_6) == 0) {
                    set_text_arg_i_1018_b472(0, g_cur_colony_rec->stock[i]);
                    popup_say(0, g_goods_name_word[i]);
                    set_name_slot_1018_b3f6(1, g_cur_colony_rec->name);
                    popup_say(2, g_nation_name_word[g_cur_colony_rec->nation]);
                    popm("TUTORIAL6", 0);
                    g_game.hint_flags |= TUT_SHOWN_6;
                }
            }
        }
    }
    while (g_depletion_rolls != 0) {
        if (rand_range(0, g_game.difficulty + 1) != 0) {
            g_cur_colony_rec->mine++;
            if (g_cur_colony_rec->mine >= 0x32) {
                g_cur_colony_rec->mine = g_cur_colony_rec->mine - 0x32;
                colony_depletion();
            }
        }
        g_depletion_rolls--;
    }
    if (g_msg_seen != 0)
        ColonyWin_Show(&g_colony_window, g_cur_colony_idx);
out:
    g_new_building = -1;
}

