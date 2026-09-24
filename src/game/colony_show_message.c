/* colony_show_message -- shipped at 1010:b048.
 *
 * Ported by tools/port.py from matched/game/colony_show_message.c.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void set_current_colony(col_i16 c);
extern void ColChild_ActivateMessage(void *w, col_i16 kind, col_i32 ms);
extern void ColChild_DrawAnyMessage(void *w, col_i16 mode, col_i32 arg);

void colony_show_message(ColChild *w, col_i16 kind, col_i32 ms)
{
    set_current_colony(w->colony);
    ColChild_ActivateMessage(w, kind, ms);
    ColChild_DrawAnyMessage(w, 1, 0L);
}
