/* port_blit_or_copy -- shipped at 1068:b188.
 *
 * Ported by tools/port.py from matched/gfx/port_blit_or_copy.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1068_d86d(Port *p, POINT *r);

extern void CopyToPort(void *src, void *dst, POINT *sr, POINT *dr);
extern void CopyToScreen(void *p, void *src, POINT *dst, POINT *at);

/* same object: 4-byte `push cs; call ` */


void port_blit_or_copy(Port *p, POINT *r)
{
    if ((*((char *)((unsigned char *)(p) + 0xfa))) != 0) {
        CopyToPort((*((void * *)((unsigned char *)(p) + 0xfc))), p, r, r);
        CopyToScreen(p, ((char *)((unsigned char *)(p) + 0x26)), r, r);
    } else {
        copy_rect_to_screen_1068_d86d(p, r);
    }
}
