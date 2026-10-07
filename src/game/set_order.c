/* set_order: candidate for 1028:c568 (98 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Writes nibble i of the current trade-route stop -- the high half for an odd index, the
 * low half for an even one. The write half of the pair with `get_order`, and the
 * twelve-slot version of `set_num_orders`, which does the same thing to
 * the single packed byte at +2.
 *
 * The two SEG12 call sites use it and `get_order` together as `set(k, get(k + 1))`
 * down a loop, which is a delete-and-shift over the nibble list.
 *
 * `orders_pointer` is NEAR-called from here -- `0E E8`, the 4-byte same-object form --
 * and is stubbed in-file to record that. It does not have to be: declared `extern` this
 * grades RELOCATABLE too, because `bytematch.compare` treats the -to-conversion as
 * a placement decision and re-checks the displacements by where they land. The stub is the
 * convention, not the requirement.
 *
 * The mask is an assignment and the merge is a compound assignment, and the two are not
 * interchangeable. `b = b & 0xf` gives load / AND / store, which is what the shipped code
 * has; `b |= v << 4` gives `OR [bp-7],AL` straight to memory, where `b = b | (v << 4)`
 * would compute into AL and store. Four bytes either way, twice.
 *
 * RENAMED 2026-09-26 from route_stop_nibble_set: sets goods slot i of the current stop: slots 0..5 unload, 6..11 load. ReportWin_TradeRoutes draws "Unload Cargo" at x+0xe1 and "Load Cargo" at x+0x1a9, then
 * ReportWin_TradeRoute1 draws count(0) goods from slots 0..5 at x 231 and count(1) goods from slots
 * 6..11 at x 431 -- under those headings. So kind 0 is UNLOAD and kind 1 is LOAD.
 *
 * RENAMED 2026-09-26 from route_stop_goods_set: the Mac build's set_order(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between set_num_orders and new_dest, which are already bound, paired in order; the arity agrees and the Mac body is 0.90x this one.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */
extern char *orders_pointer(int i);

void set_order(register int i, int goods)
{
    char *p;
    unsigned char b;

    p = orders_pointer(i);
    b = *p;
    if (i & 1) {
        b = b & 0xf;
        b |= goods << 4;
    } else {
        b = b & 0xf0;
        b |= goods & 0xf;
    }
    *p = b;
}
