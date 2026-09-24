/* format_save_filename -- shipped at 1008:b271.
 *
 * Ported by tools/port.py from matched/game/format_save_filename.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_year;                               SEG20:0x778c
 *   extern char g_save_prefix_eng[];                     DGROUP:0x07fd
 *   extern char g_save_prefix_fra[];                     DGROUP:0x0801
 *   extern char g_save_prefix_spa[];                     DGROUP:0x0805
 *   extern char g_save_prefix_dut[];                     DGROUP:0x0809
 *   extern char g_save_ext[];                            DGROUP:0x080d
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern void LTOA1(col_i16 value, char *s, col_i16 radix);








void format_save_filename(char *dst)
{
    char num[10];
    char buf[0x50];

    dst[0] = 0;
    switch (g_player_nation) {
    case 0: strcpy(buf, g_save_prefix_eng); break;
    case 1: strcpy(buf, g_save_prefix_fra); break;
    case 2: strcpy(buf, g_save_prefix_spa); break;
    case 3: strcpy(buf, g_save_prefix_dut); break;
    }
    LTOA1(g_year, num, 10);
    strcat(buf, num);
    strcat(buf, g_save_ext);
    strcpy(dst, buf);
}
