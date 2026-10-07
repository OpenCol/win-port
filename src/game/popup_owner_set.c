/* popup_owner_set -- shipped at 1050:0000.
 *
 * Ported by tools/port.py from matched/game/popup_owner_set.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_popup_owner;                      DGROUP:0x24fc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void popup_owner_set(void *v)
{
    g_popup_owner = v;
}
