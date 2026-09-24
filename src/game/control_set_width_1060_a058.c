/* control_set_width_1060_a058 -- shipped at 1060:a058.
 *
 * Ported by tools/port.py from matched/game/control_set_width_1060_a058.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: width -> width_7a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void scroll_get_range_ex_1060_ae1d(void *o, col_i16 *pa, col_i16 *pb);

void control_set_width_1060_a058(Rec *o, col_i16 v)
{
    col_i16 a;
    col_i16 b;

    scroll_get_range_ex_1060_ae1d(o, &a, &b);
    if (b - a >= v)
        o->width_7a = v;
}
