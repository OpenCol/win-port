/* copy_rect_to_screen_1010_e6a4 -- shipped at 1010:e6a4.
 *
 * Ported by tools/port.py from matched/game/copy_rect_to_screen_1010_e6a4.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void port_validate_rect_1010_edab(void *port, void *rect);

extern void CopyToScreen(Win *w, void *port, void *src, void *dst);
/* same object: 4-byte `push cs; call ` (rule 47) */

void copy_rect_to_screen_1010_e6a4(Win *w, void *r)
{
    if (r != 0) {
        CopyToScreen(w, ((char *)((unsigned char *)(w) + 0x26)), r, r);
        port_validate_rect_1010_edab(((char *)((unsigned char *)(w) + 0x26)), r);
    } else {
        CopyToScreen(w, ((char *)((unsigned char *)(w) + 0x26)), ((char *)((unsigned char *)(w) + 0x10)), ((char *)((unsigned char *)(w) + 0x10)));
        port_validate_rect_1010_edab(((char *)((unsigned char *)(w) + 0x26)), ((char *)((unsigned char *)(w) + 0x10)));
    }
}
