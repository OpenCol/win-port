/* setup_purchase: candidate for 1008:8169 (107 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Writes all five fields of one six-byte record in the EUROPE RECRUIT TABLE, four chars and
 * an int. It is the filler, and it settles where the record starts.
 *
 * Four files read that table as `g_recruit` based at SEG20:0xd83d, with `type` at +0 and
 * `price` at +3. This function writes a record based at 0xd83c, and writes ALL of it -- so the
 * byte at 0xd83c belongs to the record and the readers' base was one too high. Both spellings
 * reach the same bytes (0xd83d + 6i either way), which is why nothing ever caught it: what
 * moved is only where the boundary is, and the filler is the thing that knows.
 *
 * RENAMED 2026-09-27 from recruit_set: BEYOND THE PROPOSALS, the neighbour of init_data. Mac setup_purchase(short x6), CODE 26 0x4544, 118 bytes, called only by init_data (6 times). 8169 (int a, int type, int price, int amount, int d, int i): six arguments, 107 bytes, called only by building_table_init, 6 times. Leaf on both. Layout: second of the triple on both. Descriptive old name. tools/check_record_aliases.py:122 mentions recruit_set in a comment only. No collision.
 *
 * `a`, `type`, `amount` and `d` were `char` here. Respelled as `int`, the Mac's `short`, the
 * function still grades RELOCATABLE, so the width is free in the bytes and takes the original
 * source's type (substitution, 2026-09-28). The record's fields stay bytes.
 */

#include "colonization.h"

#include "records.h"

void setup_purchase(int a, int type, int price, int amount, int d, int i)
{
    g_recruit[i].f0 = a;
    g_recruit[i].type = type;
    g_recruit[i].tax_cut = amount;
    g_recruit[i].price = price;
    g_recruit[i].f3 = d;
}
