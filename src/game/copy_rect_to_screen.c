/* copy_rect_to_screen -- shipped at 1048:d963.
 *
 * Ported by tools/port.py from matched/game/copy_rect_to_screen.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void CopyToScreen(Win *w, void *port, void *src, void *dst);
extern void port_validate_rect(void *port, void *rect);

void copy_rect_to_screen(Win *w, void *r)
{
    if (r != 0) {
        CopyToScreen(w, ((char *)((unsigned char *)(w) + 0x26)), r, r);
        port_validate_rect(((char *)((unsigned char *)(w) + 0x26)), r);
    } else {
        CopyToScreen(w, ((char *)((unsigned char *)(w) + 0x26)), ((char *)((unsigned char *)(w) + 0x10)), ((char *)((unsigned char *)(w) + 0x10)));
        port_validate_rect(((char *)((unsigned char *)(w) + 0x26)), ((char *)((unsigned char *)(w) + 0x10)));
    }
}
