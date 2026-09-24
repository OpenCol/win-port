/* colony_goods_message -- shipped at 1010:b0c9.
 *
 * Ported by tools/port.py from matched/game/colony_goods_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
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




void colony_goods_message(ColChild *w, col_i16 n, col_i32 ms)
{
    set_current_colony(w->colony);
    ColChild_CancelMessage(w, 1);
    colony_add_goods_text(w, n);
    colony_show_message(w, 3, ms);
}
