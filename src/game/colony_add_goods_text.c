/* colony_add_goods_text -- shipped at 1010:b08e.
 *
 * Ported by tools/port.py from matched/game/colony_add_goods_text.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_colony_message_words[];             SEG20:0xd996
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void set_current_colony(col_i16 c);
extern void ColChild_AddToMessage(void *w, col_i16 idx);

void colony_add_goods_text(ColChild *w, col_i16 n)
{
    set_current_colony(w->colony);
    ColChild_AddToMessage(w, g_colony_message_words[n]);
}
