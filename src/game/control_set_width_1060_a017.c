/* control_set_width_1060_a017 -- shipped at 1060:a017.
 *
 * Ported by tools/port.py from matched/game/control_set_width_1060_a017.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: width -> width_78
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void scroll_get_range_ex_1060_adea(void *o, void *pa, void *pb);

void control_set_width_1060_a017(Rec *o, col_i16 v)
{
    col_i16 a;
    col_i16 b;

    scroll_get_range_ex_1060_adea(o, &a, &b);
    if (b - a >= v)
        o->width_78 = v;
}
