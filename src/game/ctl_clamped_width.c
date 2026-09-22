/* ctl_clamped_width -- shipped at 1050:0ad6.
 *
 * Ported by tools/port.py from matched/game/ctl_clamped_width.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: text -> value
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ctl_line_height(char *s);
col_i16 default_width(Ctl *c);
col_i16 max_int_1050_836b(col_i16 a, col_i16 b);

extern col_i16 * min_ptr_1050_979f(col_i16 *a, col_i16 *b);

/* same object: 4-byte `push cs; call ` */




col_i16 ctl_clamped_width(Ctl *c)
{
    col_i16 w;
    col_i16 r;
    col_i16 scaled;

    w = default_width(c);
    w = *min_ptr_1050_979f(&w,
            (scaled = (ctl_line_height(c->value) * 5) >> 2, &scaled));
    r = max_int_1050_836b(w, ctl_line_height(c->value));
    return r;
}
