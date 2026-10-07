/* alert_box_1070_66cf -- shipped at 1070:66cf.
 *
 * Ported by tools/port.py from matched/platform/alert_box_1070_66cf.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal MessageBox(col_u16 hwnd, char *text, char *caption, col_u16 type);
#endif

col_i16 alert_box_1070_66cf(char *caption, char *text, col_i16 icon, col_i16 buttons)
{
    register col_i16 style;
    register col_i16 r;
    col_i16 id;

    style = 0;
    switch (icon) {
    case 0:
        style = 0x40;
        break;
    case 1:
        style = 0x20;
        break;
    case 2:
        style = 0x30;
        break;
    case 3:
        style = 0x10;
        break;
    }
    switch (buttons) {
    case 4:
        style |= 4;
        break;
    case 8:
        style |= 3;
        break;
    case 0x10:
        style |= 2;
        break;
    case 1:
        break;
    case 2:
        style |= 1;
        break;
    case 0x20:
        style |= 5;
        break;
    }
    id = MessageBox(0, text, caption, style | 0x2000);
    switch (id) {
    case 1:
        r = 0;
        break;
    case 2:
        r = 1;
        break;
    case 6:
        r = 2;
        break;
    case 7:
        r = 3;
        break;
    case 4:
        r = 5;
        break;
    case 3:
        r = 4;
        break;
    case 5:
        r = 6;
        break;
    }
    return r;
}
