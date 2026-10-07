/* perform_orders_fortify: candidate for 1028:b1cc (44 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets unit n's byte +8 to 6 and refreshes its rating.
 *
 * RENAMED 2026-09-26 from unit_set_orders_6: sets unit n's orders to ORDERS_FORTIFIED (6) and refreshes its move points.
 *
 * RENAMED 2026-09-26 from unit_fortify: the Mac build's perform_orders_fortify(short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between perform_orders_move_to and in_port, where the Mac gap holds 1 functions and the Windows one 2, so the pairing inside it is by size and arity (the Mac body is 1.50x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern void end_unit_turn(int unit);

void perform_orders_fortify(int unit)
{
    int n = unit;

    g_units[n].orders = ORDERS_FORTIFIED;
    end_unit_turn(n);
}
