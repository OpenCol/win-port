/* add_nation_money_to_message -- shipped at 1048:11ff.
 *
 * Ported by tools/port.py from matched/game/add_nation_money_to_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ColChild_AddBucksToMessage(void *w, col_i32 v);

extern col_i32 nation_gold(col_i16 n);



void add_nation_money_to_message(void *w, col_i16 n)
{
    ColChild_AddBucksToMessage(w, nation_gold(n));
}
