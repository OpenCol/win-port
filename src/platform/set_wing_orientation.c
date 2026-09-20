/* set_wing_orientation -- shipped at 1070:a637.
 *
 * Ported by tools/port.py from matched/platform/set_wing_orientation.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_wing_orientation;                       DGROUP:0x39b4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_wing_orientation(int n)
{
 g_wing_orientation = n;
}
