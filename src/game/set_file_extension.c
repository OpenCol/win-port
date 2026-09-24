/* set_file_extension -- shipped at 1040:67d8.
 *
 * Ported by tools/port.py from matched/game/set_file_extension.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_dot[];                                 DGROUP:0x1d91
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern char * STRUPR(char *s);
extern char * strrchr_rtl(const char *s, col_i16 c);


void set_file_extension(char *dst, char *src, char *ext)
{
    char *p;

    if (src != dst)
        strcpy(dst, src);
    p = strrchr_rtl(dst, '.');
    if (p != 0)
        *p = 0;
    strcat(dst, g_dot);
    strcat(dst, ext);
    STRUPR(dst);
}
