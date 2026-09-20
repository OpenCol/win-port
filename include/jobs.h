/* jobs.h -- professions: a colonist's specialty, the job a colonist works, a docks recruit.
 *
 * Source: the game's JOB list resource, one profession a line, whose index is the id; the
 * JOB0..JOB27 description resources follow the same order. tools/check_headers.py re-reads it.
 * The names are the list's singular first column ("Farmer"), because the same id is both what a
 * colonist IS and what a colonist DOES: the colony's job byte for a fisherman is JOB_FISHERMAN
 * (colony_draw_work_area), and the two mines are JOB_ORE_MINER and JOB_SILVER_MINER (deplete_mine).
 *
 * ONE ID SPACE, THREE PLACES it is stored:
 *   - the specialty, Unit+0x17 for a unit and Colony+0x40+i for a colony colonist
 *     (colonist_specialty_get);
 *   - the job, Colony+0x20+i. Ids below JOB_COLONIST are the jobs a building or a tile rewards;
 *     colonist_move_kind and job_verdict split on JOB_COLONIST;
 *   - the three colonists waiting on the Europe docks, the nation record's recruit[0..2] at
 *     +2..+4 (europe_init_nations, pick_recruit, europe_nation_turn).
 *
 * WHAT CAN BE ON THE DOCKS. A new game puts JOB_INDENTURED_SERVANT (JOB_CRIMINAL on Viceroy)
 * first, and a human player on Discoverer gets JOB_CARPENTER, JOB_FARMER and JOB_SCOUT. Spain's
 * first is always JOB_MISSIONARY. Later recruits come from pick_recruit: JOB_CRIMINAL,
 * JOB_INDENTURED_SERVANT or JOB_COLONIST_ALT by the dice, or a pool draw below
 * JOB_INDENTURED_SERVANT with seven ids folded away. So the pool never produces a planter,
 * trapper, teacher, colonist or dragoon, and JOB_CONVERT never comes from Europe.
 */
#ifndef JOBS_H
#define JOBS_H

#define JOB_FARMER                  0
#define JOB_SUGAR_PLANTER           1
#define JOB_TOBACCO_PLANTER         2
#define JOB_COTTON_PLANTER          3
#define JOB_FUR_TRAPPER             4
#define JOB_LUMBERJACK              5
#define JOB_ORE_MINER               6
#define JOB_SILVER_MINER            7
#define JOB_FISHERMAN               8
#define JOB_DISTILLER               9
#define JOB_TOBACCONIST             10
#define JOB_WEAVER                  11
#define JOB_FUR_TRADER              12
#define JOB_CARPENTER               13
#define JOB_BLACKSMITH              14
#define JOB_GUNSMITH                15
#define JOB_PREACHER                16
#define JOB_STATESMAN               17
#define JOB_TEACHER                 18
#define JOB_COLONIST                19
#define JOB_PIONEER                 20
#define JOB_SOLDIER                 21
#define JOB_SCOUT                   22
#define JOB_DRAGOON                 23
#define JOB_MISSIONARY              24
#define JOB_INDENTURED_SERVANT      25      /* the list says "Ind. Servant" */
#define JOB_CRIMINAL                26
#define JOB_CONVERT                 27

/* THE OUTDOOR/INDOOR BOUNDARY. Jobs 0..8 are worked on a TILE -- farmer, the three planters, fur
 * trapper, lumberjack, the two miners and the fisherman -- and 9 upward are worked in a BUILDING,
 * starting with the distiller. `clear_colonist_specialty` tests `job >= 9 && final_job < 9`, which
 * is a crossing test and only means anything if 9 is a category edge, and `colony_assign_job`
 * branches on `job >= 9` when it places a colonist. The names come from the JOB resource, so the
 * grouping is the resource's own order rather than a reading of it. */
#define JOB_INDOOR_FIRST            9

/* THE SECOND BOUNDARY, and it falls on a real id. Ids below JOB_COLONIST are the jobs a building
 * or a tile rewards; from JOB_COLONIST up they are what a person is rather than what a colony
 * pays for. `colonist_move_kind` and `job_verdict` split on it, and `colony_assign_job` refuses
 * anything at or above it. No separate boundary name is minted for 19: the split IS the colonist
 * id, and inventing a second name for one value would add a name nothing can confirm. */

#define JOB_COUNT                   28

/* 0x1c is NOT in the JOB resource, but the code stores it and reads it as a plain colonist everywhere
 * we have looked. The portrait, `fold_spec_1c` and the UNREST message all turn it into
 * JOB_COLONIST first. `create_unit` gives it to every new land unit that has no cargo holds (and
 * JOB_SOLDIER to Continental Army and Continental Cavalry). `is_ordinary_job` groups it with the colonist, servant, criminal and convert.
 * A recruit roll that William Brewster denies (JOB_CRIMINAL or JOB_INDENTURED_SERVANT)
 * becomes it. One difference is known: a teacher reads it as JOB_INDENTURED_SERVANT (job_verdict).
 * Why the game keeps a second colonist id is not established, so the name only says it is one. */
#define JOB_COLONIST_ALT            0x1c

/* ---- the 8-byte profession record, SEG20:0xd61a, one per JOB_* id ------------------------------
 * The data segment ships as zeros; the loader fills this from the same JOB list resource the ids
 * come from, and `load_data_tables` reads the four columns STRAIGHT ACROSS -- two text values
 * then two numbers, `name`, `expert`, `school`, `price` into +0, +2, +4, +6 -- which is the
 * second witness for the split below and the one that needs no inference:
 *
 *     Farmer,          Expert Farmers,          1, 1100
 *     Sugar Planter,   Master Sugar Planters,   2,   -1
 *
 *   +0  name    the singular profession, "Farmer" -- what the colony job picker and the pedia
 *               list show (colony_assign_job, pedia_fill_list over all 28, skipping 0x12)
 *   +2  expert  the plural expert title, "Expert Farmers" -- what the buildings article prints
 *               beside the expert sprite. TWO TEXT IDS, and four files called both `name`.
 *   +4  school  WHICH SCHOOL CAN TEACH IT, which is what `job_warning` reads it for and what
 *               named it: `school == 2` wants a BUILDING_COLLEGE, `school == 3` a
 *               BUILDING_UNIVERSITY, and `school > 3` refuses outright. The resource agrees --
 *               1 for the outdoor trades, 2 for the planters and smiths, 3 for Preacher,
 *               Statesman and Missionary, 4 for the five that cannot be taught at all
 *               (Teacher, Colonist, Ind. Servant, Criminal, Convert).
 *   +6  price   the training price, -1 where there is none. Farmer 1100, Statesman 1900,
 *               Soldier 2000. `ai_manage_colony` tests `g_spec_recs[0].price` against the gold
 *               and then pays `g_spec_recs[pick].price`, which is the shipped bug recorded there.
 */
#define SCHOOL_SCHOOLHOUSE          1
#define SCHOOL_COLLEGE              2
#define SCHOOL_UNIVERSITY           3
#define SCHOOL_NONE                 4   /* `school > SCHOOL_UNIVERSITY`: cannot be taught */

#endif
