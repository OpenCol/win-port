/* MSCloseFile -- shipped at 1070:7fa6.
 *
 * Ported by tools/port.py from matched/platform/MSCloseFile.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal _lclose(col_i16 h);;
#endif

/* OFSTRUCT at +4 */


col_i16 MSCloseFile(MSFile *f)
{
    if (f->handle == 0)
        return 0;
    if (_lclose(f->handle) == -1)
        return 0;
    else {
        f->handle = 0;
        return 1;
    }
}
