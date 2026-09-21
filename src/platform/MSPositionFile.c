/* MSPositionFile -- shipped at 1070:808d.
 *
 * Ported by tools/port.py from matched/platform/MSPositionFile.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal _llseek(col_i16 h, col_i32 off, col_i16 origin);;
#endif

/* OFSTRUCT at +4 */


col_i16 MSPositionFile(MSFile *f, col_i32 off)
{
    if (f->handle == 0)
        return 0;
    if (_llseek(f->handle, off, 0) == -1L)
        return 0;
    else
        return 1;
}
