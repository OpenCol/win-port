/* activate_and_draw_message -- shipped at 1048:1471.
 *
 * Ported by tools/port.py from matched/game/activate_and_draw_message.c.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ColChild_ActivateMessage(ColChild *w, char kind, col_i32 arg);
void ColChild_DrawAnyMessage(ColChild *w, col_i16 mode, col_i32 arg);

void activate_and_draw_message(ColChild *w, char kind, col_i32 arg)
{
    ColChild_ActivateMessage(w, kind, arg);
    ColChild_DrawAnyMessage(w, 1, 0L);
}
