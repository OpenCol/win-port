/* win_set_focus_1068_e278 -- shipped at 1068:e278.
 *
 * Ported by tools/port.py from matched/gfx/win_set_focus_1068_e278.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *hwnd);

void win_set_focus_1068_e278(Win *w)
{
    MSSetFocus((*((void * *)((unsigned char *)(w) + 0x6))));
}
