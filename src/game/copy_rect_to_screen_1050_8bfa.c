/* copy_rect_to_screen_1050_8bfa -- shipped at 1050:8bfa.
 *
 * Ported by tools/port.py from matched/game/copy_rect_to_screen_1050_8bfa.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void port_validate_rect_1050_94d4(void *port, void *rect);

extern void CopyToScreen(Win *w, void *port, void *src, void *dst);
/* same object: 4-byte `push cs; call ` (rule 47) */

void copy_rect_to_screen_1050_8bfa(Win *w, void *r)
{
    if (r != 0) {
        CopyToScreen(w, ((char *)((unsigned char *)(w) + 0x26)), r, r);
        port_validate_rect_1050_94d4(((char *)((unsigned char *)(w) + 0x26)), r);
    } else {
        CopyToScreen(w, ((char *)((unsigned char *)(w) + 0x26)), ((char *)((unsigned char *)(w) + 0x10)), ((char *)((unsigned char *)(w) + 0x10)));
        port_validate_rect_1050_94d4(((char *)((unsigned char *)(w) + 0x26)), ((char *)((unsigned char *)(w) + 0x10)));
    }
}
