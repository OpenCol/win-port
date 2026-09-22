/* set_long_24fc -- shipped at 1050:0000.
 *
 * Ported by tools/port.py from matched/game/set_long_24fc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long g_popup_owner;                           DGROUP:0x24fc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_long_24fc(col_i32 v)
{
    g_popup_owner = v;
}
