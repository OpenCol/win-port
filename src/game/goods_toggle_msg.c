/* goods_toggle_msg -- shipped at 1010:b11a.
 *
 * Ported by tools/port.py from matched/game/goods_toggle_msg.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int g_stub_a;                                 
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_add_goods_text(ColChild *w, col_i16 n);
void colony_show_message(ColChild *w, col_i16 kind, col_i32 ms);

extern void set_current_colony(col_i16 c);
extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void ColChild_AddToMessage(void *w, col_i16 idx);




void goods_toggle_msg(ColChild *w, col_i16 goods, col_i16 unused, col_i16 flag)
{
    set_current_colony(w->colony);
    ColChild_CancelMessage(w, 1);
    if (flag != 0)
        colony_add_goods_text(w, 0xf);
    else
        colony_add_goods_text(w, 0x10);
    ColChild_AddToMessage(w, g_goods_name_word[goods]);
    colony_show_message(w, 1, 0L);
}
