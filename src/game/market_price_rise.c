/* market_price_rise: candidate for 1048:4ef7 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The European price of good g goes up one. The twin of `market_price_fall`, and the
 * write side of the byte `europe_ask_price` and `europe_bid_price` read.
 *
 * Unlike the fall there is no clamp -- nothing here caps the price, because the caller
 * that drives it (the volume check in SEG10 at 0x4753) tests `price[g] < cap` itself.
 *
 * IT IS NOT `price[g]++`. That gives a one-byte `INC BYTE [es:bx+0x4c]` and the whole
 * function collapses to 25 of the shipped 41. The shipped code loads the byte, sign
 * extends it with `CBW`, adds one in SIXTEEN bits and stores the low half back -- which
 * is an `int` temporary, written out as the same three statements its twin uses. The
 * difference between the two is only that `market_price_fall` has a call in the middle,
 * so its `p` needs a stack slot; here `p` lives in DX and the frame stays empty.
 */

#include "colonization.h"

#include "records.h"

void market_price_rise(int goods)
{
    int p;

    p = g_cur_nation_rec->price[goods];
    p++;
    g_cur_nation_rec->price[goods] = (signed char)p;
}
