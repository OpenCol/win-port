/* check_mask_reset -- shipped at 1050:63b6.
 *
 * Ported by tools/port.py from matched/game/check_mask_reset.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_check_mask;                             DGROUP:0x2514
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void check_mask_reset(void)
{
    g_check_mask = 0;
}
