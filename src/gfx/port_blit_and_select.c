/* port_blit_and_select -- shipped at 1068:aa8f.
 *
 * Ported by tools/port.py from matched/gfx/port_blit_and_select.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * port_bits(void *p);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal CopyRect(RECT *dst, RECT *src);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
extern void port_select_if_changed(void *p, void *page);

/* same object: 4-byte `push cs; call ` */


void port_blit_and_select(Obj *o, void *src, RECT *sr, RECT *dr)
{
    CopyRect(&(*((RECT *)((unsigned char *)(o) + 0xee))), dr);
    CopyToPort(src, ((char *)((unsigned char *)(o) + 0xc4)), sr, dr);
    port_select_if_changed(((char *)((unsigned char *)(o) + 0xc4)), port_bits(((char *)((unsigned char *)(o) + 0x26))));
}
