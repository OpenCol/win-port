/* nations.h -- the twelve-entry nation index.
 *
 * Sources: docs/findings/nation-index.md (4 European records of 316 bytes, then 8 native
 * records of 78) and docs/findings/european-nation-index.md (the order of both groups, from the
 * COUNTRY resource and the nation-advantage text).
 */
#ifndef NATIONS_H
#define NATIONS_H

#define NATION_ENGLAND          0
#define NATION_FRANCE           1
#define NATION_SPAIN            2
#define NATION_NETHERLANDS      3
#define NATION_INCAS            4
#define NATION_AZTECS           5
#define NATION_ARAWAKS          6
#define NATION_IROQUOIS         7
#define NATION_CHEROKEE         8
#define NATION_APACHE           9
#define NATION_SIOUX            10
#define NATION_TUPI             11

#define NATION_EUROPEAN_COUNT   4       /* index < 4 is European, the boundary the code tests */
#define NATION_NATIVE_COUNT     8
#define NATION_COUNT            12
#define NATION_NONE             (-1)

#define NATION_EUROPEAN_SIZE    0x13c   /* bytes per European record */
#define NATION_NATIVE_SIZE      0x4e    /* bytes per native record */

/* ---- the four money longs of the European record at SEG20:0xc2f4 ---------------------------
 * Four longs sit together at +0x22..+0x2e and were being confused for one another, because
 * `treasury` is a fair word for at least two of them. What separates them is who spends them:
 *
 *   +0x22  the KING's chest for this nation. `new_turn_colony` adds the tax you pay to it,
 *          `royal_edicts` adds (difficulty * 8 + 10) a turn -- doubled at 1600, 1700
 *          and 1750 -- and SUBTRACTS 1800 each time he buys a regular, a cavalryman or a gun.
 *          The subtraction is the proof it is a balance and not a running total.
 *   +0x26  cumulative net sale revenue: `new_turn_colony` does `+= net` and nothing reads it.
 *   +0x2a  YOUR GOLD. `add_gold` is the setter and clamps it to 0..999,999;
 *          the European overflow sale credits it directly, without the clamp.
 *   +0x2e  NOT money at all: the crosses accumulated toward the next immigrant, which is why it
 *          is listed here -- one file called it `bells` and drove the whole confusion. The
 *          liberty bells are +0xc (`add_liberty` is the setter and adds to +0xc and +0xe both).
 *          `europe_new_turn` zeroes +0x2e when it passes `need` at +0x30, `europe_recruit`
 *          discounts the price by `crosses / need` and zeroes it, and the religious report
 *          prints "(%d of %d)" of the two. The colony side adds `g_prod[PROD_CROSSES]`
 *          (SEG21:0x144), the sibling of `g_prod[PROD_BELLS]` at 0x146; both draw with icon 0x39.
 *
 * These are offsets, not macros, because every reconstruction spells the record as its own
 * `typedef struct` and reaches them by member name. tools/check_field_names.py is what keeps
 * one name from meaning two of them again.
 */

#endif
