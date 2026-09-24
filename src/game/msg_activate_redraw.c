/* msg_activate_redraw -- shipped at 1048:1c79.
 *
 * Ported by tools/port.py from matched/game/msg_activate_redraw.c.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ColChild_ActivateMessage(ColChild *c, char kind, col_i32 arg);
void ColChild_DrawAnyMessage(ColChild *c, col_i16 which, col_i32 arg);

extern void set_current_nation(col_i16 n);
/* same object: 4-byte `push cs; call ` (rule 47) */


void msg_activate_redraw(ColChild *c, char kind, col_i32 arg)
{
    set_current_nation(c->nation);
    ColChild_ActivateMessage(c, kind, arg);
    ColChild_DrawAnyMessage(c, 1, 0L);
}
