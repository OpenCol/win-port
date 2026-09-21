/* port_validate_rect_1068_e34f -- shipped at 1068:e34f.
 *
 * Ported by tools/port.py from matched/gfx/port_validate_rect_1068_e34f.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSValidRect(void *hwnd, void *rect);

void port_validate_rect_1068_e34f(Port *p, void *rect)
{
    MSValidRect((*((void * *)((unsigned char *)(p) + 0x6))), rect);
}
