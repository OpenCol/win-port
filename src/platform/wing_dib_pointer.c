/* wing_dib_pointer -- shipped at 1070:a86b.
 *
 * Ported by tools/port.py from matched/platform/wing_dib_pointer.c.
 *
 * The file's own WinGRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void * pascal WinGGetDIBPointer(col_i16 hbm, void *info);
#endif

void * wing_dib_pointer(WinGRec *r)
{
    if (r == 0)
        return 0;
    return WinGGetDIBPointer(r->hbitmap, 0);
}
