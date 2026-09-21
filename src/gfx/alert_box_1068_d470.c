/* alert_box_1068_d470 -- shipped at 1068:d470.
 *
 * Ported by tools/port.py from matched/gfx/alert_box_1068_d470.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 alert_box_1070_66cf(char *caption, char *text, col_i16 icon, col_i16 buttons);

col_i16 alert_box_1068_d470(char *caption, char *text, col_i16 icon, col_i16 buttons)
{
    return alert_box_1070_66cf(caption, text, icon, buttons);
}
