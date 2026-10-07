/* ask_for_cargo: candidate for 1018:045e (130 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Marks `goods` as the colony's export (Colony+0x8d) when `flag` is set, the
 * stock is under the warehouse capacity and the SEG21:0x122 entry is
 * zero; otherwise clears the mark if it is that goods.
 *
 * RENAMED 2026-09-26 from colony_mark_export: the Mac build's ask_for_cargo(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between needs_a_building and colony_plan, which are already bound, paired in order; the arity agrees and the Mac body is 0.98x this one.
 */

#include "colonization.h"

#include "records.h"

extern int get_warehouse(void);

void ask_for_cargo(int goods, int flag)
{
    /* declared HERE, at block scope: this function reaches them with a segment
       fixup while the rest of segment 4 reads the same globals through DS, and one
       C++ module can hold both views only this way */

    if (get_warehouse() <= g_cur_colony_rec->stock[goods])
        flag = 0;
    if (g_prod[goods] != 0)
        flag = 0;
    if (flag != 0)
        g_cur_colony_rec->wanted_goods = goods;
    else if (g_cur_colony_rec->wanted_goods == (signed char)goods)
        g_cur_colony_rec->wanted_goods = -1;
}
