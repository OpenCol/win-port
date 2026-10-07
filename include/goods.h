/* goods.h -- the sixteen goods.
 *
 * Source: the game's CARGO0..CARGO15 TEXT resources. The code agrees where it can be checked:
 * the Europe market has sixteen columns indexed by goods id (europe_market_click), and
 * the goods icon table at SEG20:0x6526 is indexed the same way. tools/check_headers.py re-reads
 * the resources.
 */
#ifndef GOODS_H
#define GOODS_H

#define GOODS_FOOD          0
#define GOODS_SUGAR         1
#define GOODS_TOBACCO       2
#define GOODS_COTTON        3
#define GOODS_FURS          4
#define GOODS_LUMBER        5
#define GOODS_ORE           6
#define GOODS_SILVER        7
#define GOODS_HORSES        8
#define GOODS_RUM           9
#define GOODS_CIGARS        10
#define GOODS_CLOTH         11
#define GOODS_COATS         12
#define GOODS_TRADE_GOODS   13
#define GOODS_TOOLS         14
#define GOODS_MUSKETS       15
#define GOODS_COUNT         16
/* ---- the production array's non-goods kinds -------------------------------------------------
 * `g_prod` (SEG21:0x122) is cleared with `for (i = 0; i < 20; i++)`, so it is twenty entries:
 * the sixteen goods, then four more that are produced by a colony but never carried in a hold.
 * All four are named: the CARGO resource that load_data reads into g_goods_name_word has
 * twenty lines in the same order -- the sixteen goods, then Hammers, Crosses, Liberty Bells,
 * Flags -- and its 17 and 18 agree with the two the code established on its own (below).
 *
 * 17 is incremented once per colonist, again for a church and again for a cathedral. 18 is
 * incremented and then adjusted by Thomas Jefferson (+50%), Thomas Paine (the tax rate as a
 * percentage), a printing press (+50%) and a newspaper (doubled) -- which is the whole of the
 * game's bell arithmetic in one block, and nothing else in the program looks like it.
 *
 * 16 is HAMMERS: compute_colonist_yield gives it to JOB_CARPENTER and doubles it with a lumber mill, and
 * colony_plan assigns colonists while lumber is in stock until it is non-zero. 19 is FLAGS by
 * the CARGO line alone -- the corpus never touches it. (Both named 2026-09-26.)
 */
#define PROD_HAMMERS        16
#define PROD_CROSSES        17
#define PROD_BELLS          18
#define PROD_FLAGS          19

#define GOODS_NONE          (-1)

#endif
