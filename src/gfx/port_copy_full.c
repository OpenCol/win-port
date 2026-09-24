/* port_copy_full -- shipped at 1068:a926.
 *
 * Ported by tools/port.py from matched/gfx/port_copy_full.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
RECT * ptr_plus_16(void *p);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal CopyRect(RECT *dst, RECT *src);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` */


void port_copy_full(Obj *o)
{
    RECT r;

    CopyRect(&r, ptr_plus_16(((char *)((unsigned char *)(o) + 0xc4))));
    CopyToPort(((char *)((unsigned char *)(o) + 0xc4)), o, &r, &r);
}
