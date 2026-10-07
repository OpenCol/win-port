/* fathers.h -- the twenty-five Founding Fathers.
 *
 * Source: the game's FATHER0..FATHER24 TEXT resources, by index; the macro names drop the life dates
 * and Brebeuf's "Father". tools/check_headers.py re-reads the resources.
 *
 * The index is also a bit number in the per-nation bit array read by `has_father` -- the flags
 * of a nation are the fathers it has. Four readings pin that, each a father's documented effect:
 * the CUSTOM HOUSE (building 0x12) is buildable only with bit 3, FATHER_PETER_STUYVESANT; the five
 * factories and the arsenal need bit 0, FATHER_ADAM_SMITH (both in eligible_to_build); and
 * fur production doubles with bit 8, FATHER_HENRY_HUDSON (compute_yield); and a colony
 * that reaches three people gets its stockade free with bit 9, FATHER_LA_SALLE (set_job).
 */
#ifndef FATHERS_H
#define FATHERS_H

#define FATHER_ADAM_SMITH               0
#define FATHER_JAKOB_FUGGER             1
#define FATHER_PETER_MINUIT             2
#define FATHER_PETER_STUYVESANT         3
#define FATHER_JAN_DE_WITT              4
#define FATHER_FERDINAND_MAGELLAN       5
#define FATHER_FRANCISCO_DE_CORONADO    6
#define FATHER_HERNANDO_DE_SOTO         7
#define FATHER_HENRY_HUDSON             8
#define FATHER_LA_SALLE                 9
#define FATHER_HERNAN_CORTES            10
#define FATHER_GEORGE_WASHINGTON        11
#define FATHER_PAUL_REVERE              12
#define FATHER_FRANCIS_DRAKE            13
#define FATHER_JOHN_PAUL_JONES          14
#define FATHER_THOMAS_JEFFERSON         15
#define FATHER_POCAHONTAS               16
#define FATHER_THOMAS_PAINE             17
#define FATHER_SIMON_BOLIVAR            18
#define FATHER_BENJAMIN_FRANKLIN        19
#define FATHER_WILLIAM_BREWSTER         20
#define FATHER_WILLIAM_PENN             21
#define FATHER_JEAN_DE_BREBEUF          22
#define FATHER_JUAN_DE_SEPULVEDA        23
#define FATHER_BARTOLOME_DE_LAS_CASAS   24
#define FATHER_COUNT             25

/* ---- the 6-byte record at SEG20:0xd6fa, one per father ---------------------------------------
 * The data segment ships as zeros; `load_data` fills it from the FATHERS resource, whose
 * columns land in order: `name, cat, weight, weight, weight`.
 *
 *     Adam Smith,          0,  2,  8,  6
 *     Ferdinand Magellan,  1,  2, 10, 10
 *
 *   +0  name       text id
 *   +2  cat        the category, FATHER_CAT_* below
 *   +3  weight[3]  the pick weight at each difficulty rank, which `Century`
 *                  selects. `choose_freedom` sums the eligible fathers' weights within a
 *                  category, draws `rand_range(1, total)` and walks the draw down -- so the same
 *                  father is likelier at one difficulty than another.
 *
 * THE FIVE CATEGORIES ARE FIVE FATHERS EACH, IN ORDER, which is what makes them nameable: the
 * resource's category column runs 0 for ids 0..4, 1 for 5..9, and so on to 4 for 20..24, and each
 * block is the group the game names. `choose_freedom` offers one candidate per category.
 */
/* These index `g_founding_word[]` at SEG20:0xd790 directly, which is the FOUNDING resource --
 * six words, the five category names in this order and then "Independence" in slot 5.
 * `choose_freedom` prints one per candidate. The resource is also where these names
 * come from: they were read off the five blocks of five in the FATHERS table first, and the
 * game's own words for them turned out to be exactly those. */
#define FATHER_CAT_TRADE         0      /* Smith, Fugger, Minuit, Stuyvesant, de Witt */
#define FATHER_CAT_EXPLORATION   1      /* Magellan, Coronado, de Soto, Hudson, La Salle */
#define FATHER_CAT_MILITARY      2      /* Cortes, Washington, Revere, Drake, Jones */
#define FATHER_CAT_POLITICAL     3      /* Jefferson, Pocahontas, Paine, Bolivar, Franklin */
#define FATHER_CAT_RELIGIOUS     4      /* Brewster, Penn, Brebeuf, Sepulveda, las Casas */
#define FATHER_CAT_COUNT         5

#endif
