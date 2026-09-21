/* MSCreateFile -- shipped at 1070:7f47.
 *
 * Ported by tools/port.py from matched/platform/MSCreateFile.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal OpenFile(const char *name, void *ofs, col_u16 mode);
#endif

/* OFSTRUCT at +4 */


col_i16 MSCreateFile(const char *name, MSFile *f)
{
    f->handle = OpenFile(name, ((char *)((unsigned char *)(f) + 0x4)), 0x1002);
    if (f->handle == -1) {
        f->handle = 0;
        f->writable = 0;
        return 0;
    } else {
        f->writable = 1;
        return 1;
    }
}
