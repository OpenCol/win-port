/* set_four_bytes -- shipped at 1050:0090.
 *
 * Ported by tools/port.py from matched/game/set_four_bytes.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char g_popup_m0;                     DGROUP:0x24f8
 *   extern unsigned char g_popup_m1;                     DGROUP:0x24f9
 *   extern unsigned char g_popup_m2;                     DGROUP:0x24fa
 *   extern unsigned char g_popup_m3;                     DGROUP:0x24fb
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_four_bytes(char a, char b, char c, char d)
{
    g_popup_m1 = a;
    g_popup_m0 = b;
    g_popup_m2 = c;
    g_popup_m3 = d;
}
