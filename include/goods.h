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
 * Two of the four are established and named; 16 and 19 are NOT.
 *
 * 17 is incremented once per colonist, again for a church and again for a cathedral. 18 is
 * incremented and then adjusted by Thomas Jefferson (+50%), Thomas Paine (the tax rate as a
 * percentage), a printing press (+50%) and a newspaper (doubled) -- which is the whole of the
 * game's bell arithmetic in one block, and nothing else in the program looks like it.
 *
 * 16 is only ever read, into a display slot, and 19 is never touched in the corpus at all. What
 * they hold is not established, so they keep their numbers.
 */
#define PROD_CROSSES        17
#define PROD_BELLS          18

#define GOODS_NONE          (-1)

#endif
