/* win_build_and_attach_1068_d49c -- shipped at 1068:d49c.
 *
 * Ported by tools/port.py from matched/gfx/win_build_and_attach_1068_d49c.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * window_of_1068_d72a(Obj *o);
void event_table_clear_1068_e157(void *p);

extern void build_movie_window_1060_a56b(Obj *o, char *title, col_i16 c, col_i16 d, col_i16 e, col_i16 f, col_i16 g);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void win_set_object(Obj *o, void *p);

void win_build_and_attach_1068_d49c(Obj *o, char *title, col_i16 c, col_i16 d, col_i16 e, col_i16 g, col_i16 h)
{
    build_movie_window_1060_a56b(o, title, c, d, e, g, h);
    win_set_object(o, window_of_1068_d72a(o));
    event_table_clear_1068_e157(&((char *)((unsigned char *)(o) + 0xd))[0]);
}
