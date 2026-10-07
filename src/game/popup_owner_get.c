/* popup_owner_get -- shipped at 1050:0016.
 *
 * Ported by tools/port.py from matched/game/popup_owner_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_popup_owner;                      DGROUP:0x24fc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * popup_owner_get(void)
{
    return g_popup_owner;
}
