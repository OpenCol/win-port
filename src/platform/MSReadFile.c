/* MSReadFile -- shipped at 1070:7fed.
 *
 * Ported by tools/port.py from matched/platform/MSReadFile.c.
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

/* OFSTRUCT at +4 */


col_i16 MSReadFile(MSFile *f, void *buf, col_i32 n)
{
    if (f->handle == 0)
        return 0;
    if (_hread(f->handle, buf, n) == -1L)
        return 0;
    else
        return 1;
}
