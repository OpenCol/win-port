/* port_validate_rect_1010_edab -- shipped at 1010:edab.
 *
 * Ported by tools/port.py from matched/game/port_validate_rect_1010_edab.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSValidRect(void *hwnd, void *rect);

void port_validate_rect_1010_edab(Port *p, void *rect)
{
    MSValidRect((*((void * *)((unsigned char *)(p) + 0x6))), rect);
}
