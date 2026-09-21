/* win_set_focus -- shipped at 1060:67c3.
 *
 * Ported by tools/port.py from matched/game/win_set_focus.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *hwnd);

void win_set_focus(Win *w)
{
    MSSetFocus((*((void * *)((unsigned char *)(w) + 0x6))));
}
