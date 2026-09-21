/* port_validate_rect_1008_eba9 -- shipped at 1008:eba9.
 *
 * Ported by tools/port.py from matched/game/port_validate_rect_1008_eba9.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSValidRect(void *hwnd, void *rect);

void port_validate_rect_1008_eba9(Port *p, void *rect)
{
    MSValidRect((*((void * *)((unsigned char *)(p) + 0x6))), rect);
}
