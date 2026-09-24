/* control_set_width_1060_90da -- shipped at 1060:90da.
 *
 * Ported by tools/port.py from matched/game/control_set_width_1060_90da.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void control_call_5fbf(void *o, void *pa, void *pb);

void control_set_width_1060_90da(Rec *o, col_i16 v)
{
    col_i16 a;
    col_i16 b;

    control_call_5fbf(o, &a, &b);
    if (b - a >= v)
        o->width = v;
}
