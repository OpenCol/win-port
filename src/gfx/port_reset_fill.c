/* port_reset_fill -- shipped at 1068:aa47.
 *
 * Ported by tools/port.py from matched/gfx/port_reset_fill.c.
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

extern void GRPort_FillColor_all(void *p, col_i16 colour);

/* same object: 4-byte `push cs; call ` */


void port_reset_fill(Obj *o, unsigned char colour)
{
    CopyRect(&(*((RECT *)((unsigned char *)(o) + 0xee))), ptr_plus_16(((char *)((unsigned char *)(o) + 0xc4))));
    GRPort_FillColor_all(((char *)((unsigned char *)(o) + 0xc4)), colour);
}
