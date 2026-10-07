/* set_ranks: candidate for 1030:3699 (228 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Scores the four European nations -- a byte, twice another byte, money / 100 and a
 * per-nation word -- sorts the nation ids by score, and writes each nation's rank.
 *
 * `g_rank_ids` IS AT SEG25:0x0, and SEG25 TILES EXACTLY, which is what proves it. The segment
 * is 80 bytes of zeros with no relocations, and four objects fill it end to end with no gap:
 * `g_rank_ids[4]` at 0x0 (8 bytes), `g_rank[4]` at 0x8 (8), `g_native_buy[16]` at 0x10 (32),
 * `g_native_sell[16]` at 0x30 (32) -- 8 + 8 + 32 + 32 = 80. This declaration had no address
 * until 2026-09-25 and the one slot the arithmetic leaves for it is 0x0.
 *
 * WHAT THAT RETIRES: `SEG25:0x6` was carried as a global of its own, `g_word_seg25_6`, read by
 * `is_enemy` and `negotiate_europe`. It is not a variable -- it is `g_rank_ids[3]`,
 * the last entry of the sorted id list, and both readers compare it against a nation index
 * because that is what this array holds (`g_rank_ids[i] = i` before the sort). A placeholder
 * sitting INSIDE a named array is the table-column shape, and the byte match cannot see it:
 * an element offset folds into the masked fixup, so both spellings grade.
 *
 * RENAMED 2026-09-26 from rank_nations: the Mac build's set_ranks(void), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller DoIndians; caller negotiate_europe), the runner-up gets 1, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "nations.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */

extern int g_rank_ids[4];   /* SEG25:0x0 */

extern void sort_16x16(int n, int *payload, int *key);

void set_ranks(void)
{
    int i;
    int key[4];
    int sum;
    int id;

    for (i = 0; i < NATION_EUROPEAN_COUNT; i++) {
        g_rank_ids[i] = (unsigned char)i;
        sum = 0;
        sum += g_nation_population[i];
        sum += g_nation_colony_count[i] * 2;
        sum += (int)(g_nations[i].gold / 100);
        sum += (int)g_military_power[i];
        key[i] = sum;
    }
    sort_16x16(4, g_rank_ids, key);
    for (i = 0; i < NATION_EUROPEAN_COUNT; i++) {
        id = g_rank_ids[i];
        g_rank[id] = (unsigned char)i;
    }
}
