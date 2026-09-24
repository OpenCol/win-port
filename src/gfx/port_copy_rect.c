/* port_copy_rect -- shipped at 1068:a979.
 *
 * Ported by tools/port.py from matched/gfx/port_copy_rect.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);

void port_copy_rect(Obj *o)
{
    CopyToPort(((char *)((unsigned char *)(o) + 0xc4)), o, &(*((RECT *)((unsigned char *)(o) + 0xee))), &(*((RECT *)((unsigned char *)(o) + 0xee))));
}
