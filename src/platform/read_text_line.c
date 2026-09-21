/* read_text_line -- shipped at 1070:81d5.
 *
 * Ported by tools/port.py from matched/platform/read_text_line.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal _hread(col_i16 h, void *buf, col_i32 n);;
#endif

col_i16 read_text_line(MSFile *f, char *out, col_i16 max)
{
    col_i16 n = 0;
    col_i16 ok;
    char c;

    *out = 0;
    if (f->handle == 0)
        return 0;
    ok = _hread(f->handle, &c, 1L) != -1L;
    while (ok && (col_i16)c != '\n' && n < max) {
        if ((col_i16)c != '\r') {
            *out = c;
            out++;
            n++;
        }
        ok = _hread(f->handle, &c, 1L) != -1L;
    }
    *out = 0;
    return ok;
}
