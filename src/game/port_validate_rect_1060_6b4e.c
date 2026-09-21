/* port_validate_rect_1060_6b4e -- shipped at 1060:6b4e.
 *
 * Ported by tools/port.py from matched/game/port_validate_rect_1060_6b4e.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSValidRect(void *hwnd, void *rect);

void port_validate_rect_1060_6b4e(Port *p, void *rect)
{
    MSValidRect((*((void * *)((unsigned char *)(p) + 0x6))), rect);
}
