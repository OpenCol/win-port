/* port_validate_rect_1038_e582 -- shipped at 1038:e582.
 *
 * Ported by tools/port.py from matched/game/port_validate_rect_1038_e582.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSValidRect(void *w, void *rect);

void port_validate_rect_1038_e582(Port *p, void *rect)
{
    MSValidRect((*((void * *)((unsigned char *)(p) + 0x6))), rect);
}
