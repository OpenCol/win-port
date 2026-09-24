/* MSAlertBox_1068_d44a -- shipped at 1068:d44a.
 *
 * Ported by tools/port.py from matched/gfx/MSAlertBox_1068_d44a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSAlertBox(char *text, col_i16 icon, col_i16 buttons);

void * MSAlertBox_1068_d44a(char *text, col_i16 icon, col_i16 buttons)
{
    return MSAlertBox(text, icon, buttons);
}
