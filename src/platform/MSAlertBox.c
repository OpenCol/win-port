/* MSAlertBox -- shipped at 1070:65e2.
 *
 * Ported by tools/port.py from matched/platform/MSAlertBox.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal MessageBox(col_u16 hwnd, const char *text, const char *caption, col_u16 type);
#endif

col_i16 MSAlertBox(const char *text, col_i16 icon, col_i16 buttons)
{
    col_i16 style = 0;
    col_i16 code;
    col_i16 r;

    switch (icon) {
    case 0: style = 0x40; break;
    case 1: style = 0x20; break;
    case 2: style = 0x30; break;
    case 3: style = 0x10; break;
    }
    switch (buttons) {
    case 4: style |= 4; break;
    case 8: style |= 3; break;
    case 0x10: style |= 2; break;
    case 1: break;
    case 2: style |= 1; break;
    case 0x20: style |= 5; break;
    }
    r = MessageBox(0, text, "Potentially fatal error", style | 0x2000);
    switch (r) {
    case 1: code = 0; break;
    case 2: code = 1; break;
    case 3: code = 4; break;
    case 4: code = 5; break;
    case 5: code = 6; break;
    case 6: code = 2; break;
    case 7: code = 3; break;
    }
    return code;
}
