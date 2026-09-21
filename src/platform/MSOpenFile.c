/* MSOpenFile -- shipped at 1070:7e76.
 *
 * Ported by tools/port.py from matched/platform/MSOpenFile.c.
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


col_i16 MSOpenFile(const char *name, MSFile *f)
{
    if (OpenFile(name, ((char *)((unsigned char *)(f) + 0x4)), 0x4000) != -1) {
        f->handle = OpenFile(name, ((char *)((unsigned char *)(f) + 0x4)), 2);
        if (f->handle == -1) {
            f->writable = 0;
            f->handle = OpenFile(name, ((char *)((unsigned char *)(f) + 0x4)), 0);
            if (f->handle == -1) {
                f->handle = 0;
                return 0;
            }
        } else {
            f->writable = 1;
        }
        return 1;
    } else {
        return 0;
    }
}
