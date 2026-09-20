/* tribes.h -- how a tribe feels about a nation, and the numbers that band it.
 *
 * WHERE THE NAMES COME FROM, AND WHERE THE VALUES DO NOT. Every other table in include/ is
 * re-derived from a game resource, so a wrong value is caught by something other than the byte
 * match (see the head of tools/check_headers.py). This one cannot be: the thresholds exist only
 * in the code, because they ARE code -- nothing in the resources says 75.
 *
 * So the two halves have different warrants, and the header says which is which:
 *
 *   the NAMES are the game's own words. `unit_enter_indian_village` builds the greeting by
 *   concatenation -- "VILLAGE" plus one of "WAR", "BAD", "MEDIUM", "SAVAGE", "HAPPY" -- so the
 *   resource name IS the band, and all five exist as TEXT resources 696..700 in COLTEXT0.DLL.
 *   tools/check_headers.py checks they still do.
 *
 *   the VALUES were read off that one function's `if` ladder and, when this header was written,
 *   nothing corroborated them. Two things now do, so the "suspiciously like quarters of 100"
 *   hedge that used to stand here has been earned out:
 *
 *     - `quarter_of_100` is a 55-byte function whose whole body buckets a 0..100 value
 *       at 25, 50 and 75 into 0..3. It exists for no other purpose, and `ReportWin_ReportsIndian`
 *       feeds it `table_78_word(tribe, nation)` -- this very quantity -- then takes the remainder
 *       as `(attitude - mood * 25) / 5` for the face to draw. The quartering is the scale.
 *     - the ATTITUDE resource (TEXT 17) lists five bands in order -- Content, Uneasy, Restless,
 *       Angry, War -- loaded into `g_attitude_word[]` at SEG20:0xd924 by `load_data_tables`. Four
 *       of them are what a 0..3 bucket indexes; the fifth is reached another way.
 *
 *   So the values are quarters of 100 and this header now says so.
 *
 * TWO VOCABULARIES NAME THE SAME BANDS, and the names here follow the greeting rather than the
 * scale. `unit_enter_indian_village` picks VILLAGEMEDIUM at 25, VILLAGEBAD at 50 and VILLAGEWAR at
 * 75, while ATTITUDE calls those bands Uneasy, Restless and Angry and keeps "War" for the top of
 * the range. The two disagree at 75 -- the greeting is already WAR where the scale still says
 * Angry -- so renaming these to the ATTITUDE words would contradict the resource they were
 * derived from. They keep the greeting's vocabulary and this note records the other.
 *
 * WHAT THE LADDER IS (unit_enter_indian_village, and the texts agree with the order):
 *
 *      attitude >= 0x4b                     -> VILLAGEWAR      "scalps hang threateningly"
 *      attitude >= 0x32                     -> VILLAGEBAD      "eyes you warily"
 *      attitude >= 0x19  or mood bit 7      -> VILLAGEMEDIUM   "busy in the surrounding fields"
 *      otherwise, tribe 2                   -> VILLAGESAVAGE
 *      otherwise                            -> VILLAGEHAPPY    "peacefully working"
 *
 * The same 0x4b decides more than prose: `extort_from_tribe` and `speak_with_chief` treat it as
 * the refusal point, `tribe_surprise_raid` counts a tribe hostile at it, and
 * `unit_enter_indian_village` turns ships away at it.
 *
 * ONE CAUTION. The quantity is spelled `alarm`, `attitude`, `tension` and `mood` by different
 * reconstructions, and they are not all the same field: the tribe-wide value comes from
 * `table_78_word(tribe, nation)` while `Settlement.mood[nation]` and `Settlement.anger[nation]`
 * are per-settlement bytes. These names band the TRIBE-WIDE value. Do not paste them onto a
 * settlement byte without reading the site.
 */
#ifndef TRIBES_H
#define TRIBES_H

/* ---- the tribe-wide attitude to one nation, `table_78_word(tribe, nation)` ------------------
 * Named for the TEXT resource each threshold selects. Rising value is rising hostility.
 */
#define TRIBE_ATTITUDE_MEDIUM       0x19    /* 25 -- VILLAGEMEDIUM at or above this */
#define TRIBE_ATTITUDE_BAD          0x32    /* 50 -- VILLAGEBAD */
#define TRIBE_ATTITUDE_WAR          0x4b    /* 75 -- VILLAGEWAR, and the hostility threshold */

/* ---- Settlement.anger[nation], bit 7 --------------------------------------------------------
 * NOT a magnitude: `visit_tribe_village` sets it with `+= 0x80` in the two branches where the
 * player refuses what the village asked, and `tribe_surprise_raid` counts the nation hostile when
 * it is set -- in the same loop, and on equal terms with `table_78_word(tribe, i) >= 0x4b`. So it
 * is a per-settlement grudge flag that makes a nation a target the way a war-level attitude does.
 * What the low seven bits hold is not established here.
 */
#define SETTLEMENT_ANGER_HOSTILE    0x80

/* ---- the tribe LEVEL, and the 6-byte record at SEG20:0xcb7c ---------------------------------
 * `load_data_tables` fills `g_level[]` from the LEVELS resource, five lines of three text ids:
 *
 *     Semi-Nomadic, Camp,    Camps
 *     Agrarian,     Village, Villages
 *     Advanced,     City,    Cities
 *     Civilized,    City,    Cities
 *     <Any>,        Capital, Capitals
 *
 * so a level names both itself and what its settlements are CALLED, singular and plural -- which
 * is why the same array answers "what kind of people are these" and "is that a camp or a city".
 * Slot 4 is not a fifth level: its name is `<Any>` and it supplies the capital's words, used
 * whichever level the tribe is.
 */
#define LEVEL_SEMI_NOMADIC      0
#define LEVEL_AGRARIAN          1
#define LEVEL_ADVANCED          2
#define LEVEL_CIVILIZED         3
#define LEVEL_CAPITAL_WORDS     4   /* not a level: the capital's singular and plural */
#define LEVEL_COUNT             4

#endif
