/* test_check_mask -- shipped at 1050:63fb.
 *
 * Ported by tools/port.py from matched/game/test_check_mask.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_check_mask;                             DGROUP:0x2514
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int test_check_mask(int n)
{
 return (1 << n) & g_check_mask;
}
