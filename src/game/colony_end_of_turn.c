/* colony_end_of_turn -- shipped at 1018:9476.
 *
 * Ported by tools/port.py from matched/game/colony_end_of_turn.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_cur_colony_idx;                         DGROUP:0x4ba2
 *   extern signed char g_center_kind;                    DGROUP:0x4ba6
 *   extern unsigned char g_turn_phase;                   DGROUP:0x4ba9
 *   extern char g_cur_colony_is_players;                 DGROUP:0x4baa
 *   extern char g_msg_seen;                              DGROUP:0x4bab
 *   extern int g_new_building;                           DGROUP:0x0c20
 *   extern int far g_random_seed;                        DGROUP:0x4b86
 *   extern Nation far * far g_cur_nation_rec;            DGROUP:0x4cc4
 *   extern int far g_cur_nation;                         DGROUP:0x4cc8
 *   extern char far g_map_window;                        SEG20:0x0658
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_flags_7786;                         SEG20:0x7786
 *   extern int far g_scenario_flags;                     SEG20:0x7788
 *   extern int far g_year;                               SEG20:0x778c
 *   extern int far g_season;                             SEG20:0x778e
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern unsigned char far g_mother_country_idx;       SEG20:0x77d4
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern unsigned char far g_specialty_tally[];        SEG20:0xda2a
 *   extern SpecRec far g_spec_recs[];                    SEG20:0xd61a
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_nation_name[];                      SEG20:0xd896
 *   extern int far g_nation_name_word[];                 SEG20:0xd8ae
 *   extern int far g_word_d9b8;                          SEG20:0xd9b8
 *   extern int far g_word_d9ba;                          SEG20:0xd9ba
 *   extern int far g_word_dd26;                          SEG20:0xdd26
 *   extern int far g_word_dd28;                          SEG20:0xdd28
 *   extern int far g_prod[];                             SEG21:0x0122
 *   extern int far g_crosses_this_turn;                  SEG21:0x0144
 *   extern int far g_value[];                            SEG21:0x0164
 *   extern int far g_over_lo[];                          SEG21:0x018c
 *   extern int far g_over_band[];                        SEG21:0x01b4
 *   extern unsigned char far g_goods_value[][16];        SEG30:0x0010
 *   extern char far g_text_arg_f4;                       SEG31:0x00f4
 *   extern long far g_text_args;                         SEG31:0x02d4
 *
 * The file's own Colony, Nation, NationRec52, SpecRec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: horses -> horses_stock, muskets -> f49, net_take -> f26, royal_gold -> f22
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The reconstruction marks this function `extern "C"`: Borland builds
 * the unit as C++ and that is what keeps the shipped name unmangled.
 * A host build is C, where the same words do not parse. */
#ifdef __cplusplus
#define COL_EXTERN_C extern "C"
#else
#define COL_EXTERN_C
#endif

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * build_item_name(col_i16 b);
char colony_event_message(char *msg, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f);
col_i16 bitmap_test(col_i16 n);
col_i16 build_item_cost(col_i16 b, col_i16 *out);
col_i16 build_item_kind(col_i16 b, col_i16 *out);
col_i16 building_is_useful(col_i16 n);
col_i16 colonist_job(col_i16 u);
col_i16 colonist_spec_nibble(col_i16 u);
col_i16 colonist_specialty_get(col_i16 u);
col_i16 colony_count_job(col_i16 job);
col_i16 colony_grid_metrics(col_i16 *pa, col_i16 *pb);
col_i16 colony_sol_percent(void);
col_i16 colony_warehouse_capacity(void);
col_i16 current_colony_flag_get(col_i16 bit);
col_i16 indexed_difference(col_i16 idx, col_i32 a);
col_i16 is_ordinary_job(col_i16 spec);
col_i16 max_int_1018_b34f(col_i16 a, col_i16 b);
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);
col_i32 max_long_1018_b375(col_i32 a, col_i32 b);
col_i32 min_long_1018_b3a3(col_i32 a, col_i32 b);
void colonist_set_spec_nib(col_i16 u, col_i16 n);
void colonist_specialty_set(col_i16 u, col_i16 spec);
void colony_build_complete(void);
void colony_delete(col_i16 c);
void colony_drop_colonist(col_i16 u);
void colony_native_claims(void);
void colony_refresh_screen(void);
void deplete_mine(void);
void fort_opens_fire(void);
void nation_add_gold(col_i16 nation, col_i32 amount);
void set_current_colony(col_i16 c);
void set_name_slot_1018_b3f6(col_i16 slot, char *s);
void set_text_arg_i_1018_b472(col_i16 slot, col_i16 v);

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
/* Colony+0xc2 is the bells ACCUMULATED and +0xc6 the figure they are measured
      against: `colony_sol_percent` holds both and computes `bells * 100 / bells_cap`,
      and `colony_end_of_turn` decays +0xc6 by a 64th a turn and adds twice the
      population. Twenty files had called +0xc6 `bells`, which is the OTHER field */
extern void set_current_nation(col_i16 n);
extern void add_liberty(col_i16 nation, col_i16 bells);
extern col_i16 goods_price_sell(col_i16 good);
extern void market_record_sale(col_i16 good, col_i16 amount);
extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void message_append(void *w, char *s);
extern void ColChild_AddToMessage(void *w, col_i16 word);
extern void ColChild_AddNumberToMessage(void *w, col_i16 v);
extern void ColChild_BackspaceMessage(void *w);
extern void viceroy_activate_message(col_i16 a, col_i32 b);
extern col_i16 minimax(col_i16 v, col_i16 lo, col_i16 hi);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void music_mood_store_1040_8936(col_i16 n);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern col_i16 create_unit_1038_2246(col_i16 type, col_i16 nation, col_i16 x, col_i16 y);
extern void seed_random(col_i16 seed);
extern void music_mood_if_playing(col_i16 n);
extern void music_set_mood(col_i16 n);


extern char * nth_text_string(col_i16 n);
extern void text_message_show(char *s, col_i16 n);
extern void colony_window_open(void *w, col_i16 colony);

/* same object: 4-byte `push cs; call ` (rule 47) */


































COL_EXTERN_C void colony_end_of_turn(col_i16 colony)
{
    register col_i16 u;
    col_i16 i;
    col_i16 grow;
    col_i16 unit;
    col_i16 job;
    col_i16 spec2;
    col_i16 kind;
    col_i16 need;
    col_i16 nib;
    col_i16 food0;
    col_i16 starve;
    col_i16 victim;
    col_i16 first;
    col_i16 cap;
    col_i16 over;
    col_i16 excess;
    col_i16 spoil;
    col_i16 spoil_good;
    col_i16 nspoil;
    col_i16 spec;
    col_i16 delta;
    col_i16 pick;
    col_i16 chance;
    col_i16 tools;
    col_i16 budget;
    col_i16 nation;
    col_i16 amount;
    col_i16 useful;
    col_i16 kind2;
    col_i16 item;
    col_i16 bells;
    col_i16 gross;
    col_i16 tax;
    col_i16 net;
    col_i16 sol;
    col_i16 rebel;
    col_i16 tories;
    col_i16 limit;
    col_i16 sol_before;
    col_i16 spec_list[3];
    col_i16 nteach = 0;
    col_i16 nspec = 0;
    col_i16 start[16];
    col_i16 prod[16];
    col_i16 teach[32];
    char buf[0x50];

    g_msg_seen = 0;
    set_current_colony(colony);
    nation = g_cur_colony_rec->nation;
    set_current_nation(nation);
    fort_opens_fire();
    colony_native_claims();
    colony_refresh_screen();
    bells = indexed_difference(0x12, 0L);
    add_liberty(nation, bells);
    cap = colony_warehouse_capacity();
    food0 = ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[0];
    g_cur_colony_rec->changed = 0;
    for (i = 0; i < GOODS_COUNT; i++) {
        start[i] = ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i];
        delta = indexed_difference(i, 0L);
        if (nation >= NATION_EUROPEAN_COUNT || g_nation52[nation].control != CONTROL_HUMAN)
            delta = g_prod[i] - g_value[i];
        if ((nation >= NATION_EUROPEAN_COUNT || g_nation52[nation].control != CONTROL_HUMAN) && i == 0)
            delta += g_difficulty >> 1;
        ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] += delta;
        ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] = max_int_1018_b34f(0, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i]);
        if (current_colony_flag_get(BUILDING_CUSTOM_HOUSE)) {
            if (!(g_cur_colony_rec->f1b & 3) || nation >= NATION_EUROPEAN_COUNT
                || g_nation52[nation].control != CONTROL_HUMAN) {
                useful = (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
                         ? bitmap_test(i)
                         : building_is_useful(i);
                if (useful != 0 && ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] >= 0x64) {
                    amount = ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] - 0x32;
                    ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] -= amount;
                    delta -= amount;
                    gross = goods_price_sell(i) * amount;
                    if ((g_game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                        tax = gross * (col_i32)g_cur_nation_rec->tax / 100;
                        net = gross - tax;
                    } else {
                        tax = 0;
                        net = gross;
                    }
                    nation_add_gold(g_cur_nation, (col_i32)net);
                    market_record_sale(i, amount);
                    g_cur_nation_rec->f22 += tax;
                    g_cur_nation_rec->f26 += net;
                    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
                        ColChild_CancelMessage(&g_map_window, 1);
                        message_append(&g_map_window, ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
                        ColChild_AddToMessage(&g_map_window, g_word_dd26);
                        ColChild_AddNumberToMessage(&g_map_window, amount);
                        ColChild_AddToMessage(&g_map_window, g_goods_name_word[i]);
                        ColChild_AddToMessage(&g_map_window, g_word_dd28);
                        ColChild_AddNumberToMessage(&g_map_window, gross);
                        ColChild_BackspaceMessage(&g_map_window);
                        message_append(&g_map_window, ".");
                        if ((g_game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                            ColChild_AddNumberToMessage(&g_map_window, g_cur_nation_rec->tax);
                            ColChild_BackspaceMessage(&g_map_window);
                            ColChild_AddToMessage(&g_map_window, g_word_d9b8);
                            ColChild_AddNumberToMessage(&g_map_window, tax);
                            ColChild_BackspaceMessage(&g_map_window);
                            ColChild_AddToMessage(&g_map_window, g_word_d9ba);
                            ColChild_AddNumberToMessage(&g_map_window, net);
                        }
                        viceroy_activate_message(1, 0xaL);
                    }
                }
            }
        }
        if (i != 0) {
            prod[i] = ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] - cap;
            prod[i] = minimax(prod[i], 0, delta);
        } else
            prod[i] = 0;
        if (g_prod[i] != 0 && (delta > 0 || (delta == 0 && prod[i] != 0)))
            g_cur_colony_rec->changed |= 1 << i;
    }
    sol_before = colony_sol_percent();
    if ((g_game_flags & GAMEFLAG_INDEPENDENCE) != 0 && g_cur_colony_rec->nation == g_mother_country_idx)
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
    sol = colony_sol_percent();
    set_text_arg_i_1018_b472(0, sol);
    rebel = ((g_flags_7786 & COLOPT_NO_REBEL_MAJORITY) == 0x200);
    if (sol >= 0x32 && !(g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel != 0) {
        set_name_slot_from_text(1, g_nation_name[nation]);
        g_msg_seen |= colony_event_message("REBELMAJORITY", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status |= COLONY_REBEL_MAJORITY;
    } else if (sol >= 0x32 && !(g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel == 0) {
        g_cur_colony_rec->status |= COLONY_REBEL_MAJORITY;
    } else if (sol >= 0x64 && !(g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel != 0) {
        set_name_slot_from_text(1, g_nation_name[nation]);
        g_msg_seen |= colony_event_message("REBELUNANIMOUS", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status |= COLONY_REBEL_UNANIMOUS;
    } else if (sol >= 0x64 && !(g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel == 0) {
        g_cur_colony_rec->status |= COLONY_REBEL_UNANIMOUS;
    } else if (sol < 0x5f && (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel != 0) {
        set_name_slot_from_text(1, g_nation_name[nation]);
        g_msg_seen |= colony_event_message("TORYMINORITY", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status &= ~COLONY_REBEL_UNANIMOUS;
    } else if (sol < 0x5f && (g_cur_colony_rec->status & COLONY_REBEL_UNANIMOUS) && rebel == 0) {
        g_cur_colony_rec->status &= ~COLONY_REBEL_UNANIMOUS;
    } else if (sol < 0x32 && (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel != 0) {
        set_name_slot_from_text(1, g_nation_name[nation]);
        g_msg_seen |= colony_event_message("TORYMAJORITY", !g_msg_seen, -1, 0, 0, 1, 0);
        g_cur_colony_rec->status &= ~COLONY_REBEL_MAJORITY;
    } else if (sol < 0x32 && (g_cur_colony_rec->status & COLONY_REBEL_MAJORITY) && rebel == 0) {
        g_cur_colony_rec->status &= ~COLONY_REBEL_MAJORITY;
    } else if (sol / 10 > sol_before / 10) {
        if ((g_flags_7786 & COLOPT_NO_SONS_OF_LIBERTY) != 0x100)
            g_msg_seen |= colony_event_message("SONSUP", !g_msg_seen, -1, 0, 0, 1, 0);
    } else if ((sol + 4) / 10 < sol_before / 10) {
        if ((g_flags_7786 & COLOPT_NO_SONS_OF_LIBERTY) != 0x100)
            g_msg_seen |= colony_event_message("SONSDOWN", !g_msg_seen, -1, 0, 0, 1, 0);
    }
    tories = g_cur_colony_rec->pop * (100 - sol) / 100;
    limit = 10 - g_difficulty;
    if (tories >= limit) {
        if (!(g_cur_colony_rec->status & COLONY_INEFFICIENT) && (g_flags_7786 & COLOPT_NO_BAD_GOVERNMENT) == 0) {
            set_text_arg_i_1018_b472(0, limit);
            g_msg_seen |= colony_event_message("INEFFICIENT", !g_msg_seen, -1, 0, 0, 5, 0);
        }
        g_cur_colony_rec->status |= COLONY_INEFFICIENT;
    } else {
        if ((g_cur_colony_rec->status & COLONY_INEFFICIENT) && (g_flags_7786 & COLOPT_NO_BAD_GOVERNMENT) == 0)
            g_msg_seen |= colony_event_message("EFFICIENT", !g_msg_seen, -1, 0, 0, 5, 0);
        g_cur_colony_rec->status &= ~COLONY_INEFFICIENT;
    }
    for (i = 0; i < GOODS_COUNT; i++) {
        if (i != 0) {
            prod[i] = min_int_1018_b329(prod[i], ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] - cap);
            prod[i] = max_int_1018_b34f(0, prod[i]);
        }
    }
    for (i = 0; g_cur_colony_rec->pop > i; i++) {
        nib = colonist_spec_nibble(i);
        job = colonist_job(i);
        spec = colonist_specialty_get(i);
        nib++;
        if (spec == 0x1a || spec == 0x19 || spec == 0x1c || spec == 0x13)
            teach[nteach++] = i;
        if (job == JOB_TEACHER && nspec < 3) {
            if (spec == 0x1c)
                spec = 0x19;
            kind = g_spec_recs[spec].school;
            if (kind < 4) {
                switch (kind) {
                case 1:
                    need = 4;
                    break;
                case 2:
                    need = 6;
                    break;
                case 3:
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
        colonist_set_spec_nib(i, nib);
    }
    for (i = 0; i < nspec; i++) {
        if (nteach == 0) {
            g_msg_seen |= colony_event_message("TRAINFAIL", !g_msg_seen, -1, 0, 0, 2, 0x8025);
            break;
        }
        if (g_cur_colony_is_players != 0)
            music_mood_store_1040_8936(2);
        pick = rand_range(0, nteach - 1);
        u = teach[pick];
        set_name_slot_1018_b3f6(0, ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
        spec = colonist_specialty_get(u);
        if (spec == 0x1a) {
            colonist_specialty_set(u, JOB_INDENTURED_SERVANT);
            if ((g_flags_7786 & COLOPT_NO_TRAINED) == 0)
                g_msg_seen |= colony_event_message("TRAINCRIMINAL", !g_msg_seen, -1, 0, 0, 2, 0x8025);
        } else if (spec == 0x19) {
            colonist_specialty_set(u, JOB_COLONIST_ALT);
            if ((g_flags_7786 & COLOPT_NO_TRAINED) == 0)
                g_msg_seen |= colony_event_message("TRAININDENTURED", !g_msg_seen, -1, 0, 0, 2, 0x8025);
        } else {
            colonist_specialty_set(u, spec_list[i]);
            set_name_slot_from_text(1, g_spec_recs[spec_list[i]].name);
            if ((g_flags_7786 & COLOPT_NO_TRAINED) == 0)
                g_msg_seen |= colony_event_message("TRAINPROFESSION", !g_msg_seen, -1, 0, 0, 2, 0x8025);
        }
        for (u = pick; nteach - 1 > u; u++)
            teach[u] = teach[u + 1];
        nteach--;
    }
    for (i = 0; g_cur_colony_rec->pop > i; i++) {
        job = colonist_job(i);
        spec2 = colonist_specialty_get(i);
        if (spec2 == JOB_CONVERT)
            continue;
        if (is_ordinary_job(spec2))
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
        colonist_specialty_set(i, job);
        set_name_slot_from_text(1, g_spec_recs[job].name);
        if ((g_flags_7786 & COLOPT_NO_TRAINED) == 0)
            g_msg_seen |= colony_event_message("TRAINPROFESSION", !g_msg_seen, -1, 0, 0, 2, 0x8025);
    }
    grow = colony_grid_metrics(0, 0);
    if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[0] >= grow) {
        ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[0] -= grow;
        unit = create_unit_1038_2246(UNIT_COLONISTS, nation, g_cur_colony_rec->x, g_cur_colony_rec->y);
        g_msg_seen |= colony_event_message("NEWCOLONIST", !g_msg_seen, -1, 0, 0, 4, 0);
    }
    if ((nation >= NATION_EUROPEAN_COUNT || g_nation52[nation].control != CONTROL_HUMAN) && g_over_band[0] < 3)
        g_over_band[0] = 0;
    if (g_over_band[0] != 0) {
        seed_random(g_random_seed);
        starve = 0;
        first = (g_season == 0);
        starve = 1;
        if (food0 != 0)
            starve = 0;
        if (g_difficulty <= DIFFICULTY_EXPLORER) {
            if (g_year < 0x5f0 || rand_range(0, 2 - g_difficulty) != 0)
                starve = 0;
        }
        if (starve == 0) {
            if (g_cur_colony_is_players != 0)
                music_mood_if_playing(1);
            if (first != 0)
                g_msg_seen |= colony_event_message("FOOD1", !g_msg_seen, 0, 0, 0, 5, 0);
            else
                g_msg_seen |= colony_event_message("FOOD2", !g_msg_seen, 0, 0, 0, 5, 0);
        } else if (g_cur_colony_rec->pop == starve) {
            if (g_cur_colony_is_players != 0)
                music_set_mood(1);
            g_msg_seen |= colony_event_message("VANISH", 0, -1, 0, 0, 5, 0);
        } else {
            if (g_cur_colony_is_players != 0)
                music_set_mood(1);
            if (first != 0)
                g_msg_seen |= colony_event_message("STARVE1", !g_msg_seen, 0, 0, 0, 5, 0);
            else
                g_msg_seen |= colony_event_message("STARVE2", !g_msg_seen, 0, 0, 0, 5, 0);
        }
        for (i = 0; i < starve; i++) {
            victim = rand_range(0, g_cur_colony_rec->pop - 1);
            colony_drop_colonist(victim);
        }
        if (g_cur_colony_rec->pop == 0) {
            colony_delete(g_cur_colony_idx);
            goto out;
        }
    } else if (g_over_lo[0] != 0 && ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[0] < g_over_lo[0] * 4
               && (g_flags_7786 & COLOPT_NO_FOOD_SHORTAGE) == 0) {
        if (g_cur_colony_is_players != 0)
            music_mood_store_1040_8936(1);
        set_text_arg_i_1018_b472(0, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[0]);
        g_msg_seen |= colony_event_message("FOODLOW", !g_msg_seen, -1, 0, 0, 5, 0);
    }
    if ((g_flags_7786 & COLOPT_NO_MATERIALS_SHORTAGE) == 0) {
        if (g_over_band[5] != 0 && indexed_difference(0x10, 0L) == 0)
            g_msg_seen |= colony_event_message("LUMBER", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_over_band[3] != 0 && indexed_difference(0xb, 0L) == 0)
            g_msg_seen |= colony_event_message("COTTON", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_over_band[2] != 0 && indexed_difference(0xa, 0L) == 0)
            g_msg_seen |= colony_event_message("TOBACCO", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_over_band[1] != 0 && indexed_difference(9, 0L) == 0)
            g_msg_seen |= colony_event_message("CANESUGAR", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_over_band[4] != 0 && indexed_difference(0xc, 0L) == 0)
            g_msg_seen |= colony_event_message("FURS", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_over_band[6] != 0
            && indexed_difference(0xe, 0L) - indexed_difference(0xf, 0L) == 0)
            g_msg_seen |= colony_event_message("ORE", !g_msg_seen, 0, 0, 0, 5, 0);
        if (g_over_band[14] != 0 && indexed_difference(0xf, 0L) == 0)
            g_msg_seen |= colony_event_message("TOOLS", !g_msg_seen, 0, 0, 0, 5, 0);
    }
    g_cur_colony_rec->hammers += indexed_difference(0x10, 0L);
    g_cur_colony_rec->hammers = max_int_1018_b34f(0, g_cur_colony_rec->hammers);
    if (build_item_cost(g_cur_colony_rec->item, &tools) <= g_cur_colony_rec->hammers
        && (col_i16)g_cur_colony_rec->item >= 0) {
        kind2 = build_item_kind(g_cur_colony_rec->item, &item);
        if (kind2 == 1 && current_colony_flag_get(item)) {
            g_cur_colony_rec->status |= COLONY_BUILD_DONE;
            if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
                set_name_slot_1018_b3f6(1, build_item_name(g_cur_colony_rec->item));
                g_msg_seen |= colony_event_message("ALREADYHAVE", !g_msg_seen, 2, 0, 0, 5, 0);
            }
            goto after_cap;
        }
        if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[14] < tools) {
            if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
                if ((g_flags_7786 & COLOPT_NO_TOOLS_NEEDED) == 0) {
                    set_name_slot_1018_b3f6(1, build_item_name(g_cur_colony_rec->item));
                    set_text_arg_i_1018_b472(0, tools);
                    set_text_arg_i_1018_b472(1, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[14]);
                    strcpy(buf, "NEEDTOOLS");
                    if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[14] == 0)
                        strcat(buf, "0");
                    g_msg_seen |= colony_event_message(buf, !g_msg_seen, 2, 0, 0, 5, 0);
                }
            } else
                ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[14] = tools;
        }
        if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[14] >= tools) {
            ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[14] -= tools;
            colony_build_complete();
        }
    }
    cap = colony_warehouse_capacity();
after_cap:
    g_nations[nation].crosses = g_nations[nation].crosses + g_crosses_this_turn;
    budget = (colony_count_job(5) * 2 + ((col_i16)g_center_kind == 5)) * 5 >> 1;
    if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[5] > cap) {
        over = ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[5] - cap;
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
        over = ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] - cap;
        if (over > 0) {
            if (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation52[g_cur_colony_rec->nation].control != CONTROL_HUMAN) {
                excess = over;
                if (i == 0xf) {
                    while (excess >= 0x32) {
                        excess -= 0x32;
                        g_nations[g_cur_colony_rec->nation].f49++;
                    }
                }
                if (i == 8) {
                    g_nations[g_cur_colony_rec->nation].horses_stock += excess;
                    excess = 0;
                }
                market_record_sale(i, excess);
                gross = g_goods_value[g_cur_colony_rec->nation][i] * excess;
                ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0xbc))[i] += i;
                ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0x7c))[i] += gross;
                g_cur_nation_rec->gold += gross;
            }
            if (prod[i] >= over) {
                ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] = cap;
                continue;
            }
            ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] -= prod[i];
            over -= prod[i];
            spoil = over;
            spoil = min_int_1018_b329(spoil, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i]);
            if (spoil < 2)
                spoil = 0;
            if (spoil != 0) {
                nspoil++;
                spoil_good = i;
            }
            ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] -= spoil;
        }
    }
    if (nspoil != 0) {
        if (nspoil == 1) {
            (*g_text_args) = spoil;
            strcpy(&g_text_arg_f4, nth_text_string(g_goods_name_word[spoil_good]));
            g_msg_seen |= colony_event_message("SPOIL1", !g_msg_seen, 0, 0, 0, 5, 0);
        } else
            g_msg_seen |= colony_event_message("SPOIL2", !g_msg_seen, 0, 0, 0, 5, 0);
    }
    if (g_cur_colony_is_players != 0 && (g_flags_7786 & COLOPT_NO_NEW_CARGO) == 0) {
        for (i = 0; i < GOODS_COUNT; i++) {
            if (start[i] / 100 < ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] / 100 && i != 0) {
                music_mood_store_1040_8936(2);
                set_name_slot_from_text(1, g_goods_name_word[i]);
                (*g_text_args) = cap;
                strcpy(buf, "CARGOREADY0");
                if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] == cap)
                    buf[10] = '1';
                if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] == cap && cap == 0x12c)
                    buf[10] = '2';
                g_msg_seen |= colony_event_message(buf, !g_msg_seen, -1, 0, 0, 5, 0);
                if (g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT
                    && g_nation52[g_cur_colony_rec->nation].control == CONTROL_HUMAN
                    && (g_game_flags & GAMEFLAG_TUTORIAL) && (g_scenario_flags & TUT_SHOWN_6) == 0) {
                    set_text_arg_i_1018_b472(0, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i]);
                    set_name_slot_from_text(0, g_goods_name_word[i]);
                    set_name_slot_1018_b3f6(1, ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
                    set_name_slot_from_text(2, g_nation_name_word[g_cur_colony_rec->nation]);
                    text_message_show("TUTORIAL6", 0);
                    g_scenario_flags |= TUT_SHOWN_6;
                }
            }
        }
    }
    while (g_turn_phase != 0) {
        if (rand_range(0, g_difficulty + 1) != 0) {
            g_cur_colony_rec->mine++;
            if (g_cur_colony_rec->mine >= 0x32) {
                g_cur_colony_rec->mine = g_cur_colony_rec->mine - 0x32;
                deplete_mine();
            }
        }
        g_turn_phase--;
    }
    if (g_msg_seen != 0)
        colony_window_open(&(*g_colony_window), g_cur_colony_idx);
out:
    g_new_building = -1;
}
